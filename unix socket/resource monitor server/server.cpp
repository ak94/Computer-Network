#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"
#include "sys/wait.h"
using namespace std;

#define backlog 3

static int send_file_descriptor(int socket,int fd_to_send)
{
	struct msghdr message;
	struct iovec iov[1];
	struct cmsghdr *control_message = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* We are passing at least one byte of data so that recvmsg() will not return 0 */
	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_iov = iov;
	message.msg_iovlen = 1;
	message.msg_controllen =  CMSG_SPACE(sizeof(int));
	message.msg_control = ctrl_buf;

	control_message = CMSG_FIRSTHDR(&message);
	control_message->cmsg_level = SOL_SOCKET;
	control_message->cmsg_type = SCM_RIGHTS;
	control_message->cmsg_len = CMSG_LEN(sizeof(int));

	*((int *) CMSG_DATA(control_message)) = fd_to_send;

	return sendmsg(socket, &message, 0);
}

int main(int argc, char const *argv[])
{
	bool r[3];
	for (int i = 0; i < 3; ++i)
	{
		r[i]=true;
	}
	std::vector<string> path;
	path.clear();
	path.push_back("/tmp/unix_socket1");
	path.push_back("/tmp/unix_socket2");
	path.push_back("/tmp/unix_socket3");

	int sfd,nsfd;
	struct sockaddr_in server,client;
	socklen_t clilen;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	server = {0};
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port = htons(8889);

	if(bind(sfd,(struct sockaddr *)&server,sizeof(server))<0)
	{
		perror("bind sfd error");
		exit(0);
	}
	cout<<"bind AF_INET success\n";
	listen(sfd,backlog);

	int usfd[3];
	struct sockaddr_un sender[3];
	for(int i=0;i<3;i++)
	{
		usfd[i]=socket(AF_UNIX,SOCK_STREAM,0);
		sender[i]={0};
		sender[i].sun_family=AF_UNIX;
		strncpy(sender[i].sun_path,path[i].c_str(),sizeof(sender[i].sun_path)-1);
	}

	for (int i = 0; i < 3; ++i)
	{
		if(connect(usfd[i],(struct sockaddr *)&sender[i],sizeof(struct sockaddr_un))<0)
		{
			perror("connect error "+i);
			exit(1);
		}
	}
	fd_set rfds;
	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec =0;
	int retval;


	while(1)
	{
		FD_ZERO(&rfds);
		int fdnum=-1;
		FD_SET(sfd,&rfds);
		if(fdnum<sfd)
			fdnum=sfd;

		for (int i = 0; i < 3; ++i)
		{
			FD_SET(usfd[i],&rfds);
			if(fdnum<usfd[i])
				fdnum=usfd[i];
		}

		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);

		if(retval>0)
		{
			if(FD_ISSET(sfd,&rfds))
			{
				nsfd = accept(sfd,(struct sockaddr *)&client,(socklen_t *)&clilen);
				if(nsfd<0)
				{
					perror("accept error");
					exit(1);
				}
				bool found=false;
				for (int i = 0; i < 3; ++i)
				{
					if(r[i])
					{
						if(send_file_descriptor(usfd[i],nsfd)<0)
						{
							perror("sendmsg error");
							exit(0);
						}
						cout<<"nsfd sent to "<<i+1<<"\n";
						close(nsfd);
						r[i]=false;
						found=true;
						break;
					}
				}

				if (!found)
				{
					send(nsfd,"All resources full\n",20,0);
					close(nsfd);
				}

			}

			for (int i = 0; i < 3; ++i)
			{
				if(FD_ISSET(usfd[i],&rfds))
				{
					char buffer[20];
					memset(buffer,'\0',sizeof(buffer));
					recv(usfd[i],buffer,sizeof(buffer),0);
					cout<<"gotit\n";
					r[i]=true;
					break;
				}
			}
		}else if(retval<0)
		{
			perror("select error");
			exit(1);
		}
		

	}
	return 0;
}
