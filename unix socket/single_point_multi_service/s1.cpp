#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"
#include "sys/wait.h"
using namespace std;

#define path "/tmp/unix_socket1"
#define backlog 3
static int recv_file_descriptor(int socket) /* Socket from which the file descriptor is read */
{
	int sent_fd;
	struct msghdr message;
	struct iovec iov[1];
	struct cmsghdr *control_message = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];
	int res;

	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* For the dummy data */
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_control = ctrl_buf;
	message.msg_controllen = CMSG_SPACE(sizeof(int));
	message.msg_iov = iov;
	message.msg_iovlen = 1;

	if((res = recvmsg(socket, &message, 0)) <= 0)
		return res;

 /* Iterate through header to find if there is a file descriptor */
	for(control_message = CMSG_FIRSTHDR(&message);
		control_message != NULL;
		control_message = CMSG_NXTHDR(&message,
			control_message))
	{
		if( (control_message->cmsg_level == SOL_SOCKET) &&
			(control_message->cmsg_type == SCM_RIGHTS) )
		{
			return *((int *) CMSG_DATA(control_message));
		}
	}

	return -1;
}

int main(int argc, char const *argv[])
{
	cout<<"s1";
	int usfd,csfd;
	struct sockaddr_un server,client;
	socklen_t clilen;

	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	unlink(path);
	server = {0};
	server.sun_family=AF_UNIX;
	strncpy(server.sun_path,path,sizeof(server.sun_path)-1);

	if(bind(usfd,(struct sockaddr *)&server,sizeof(struct sockaddr_un))<0)
	{
		perror("bind error");
		exit(1);
	}
	cout<<"bind usfd success\n";
	listen(usfd,backlog);
	
	clilen =sizeof(struct sockaddr_un);
	csfd = accept(usfd,(struct sockaddr *)&client,&clilen);

	if(csfd<0)
	{
		perror("accept error");
		exit(1);
	}

	cout<<"request from server\n";
	
	//read_fd(csfd,&nsfd);
	vector<int> nsfd;
	nsfd.clear();
	nsfd.push_back(recv_file_descriptor(csfd));
	cout<<nsfd[0]<<" nsfd\n";
	char buffer[2000];
	memset(buffer,'\0',sizeof(buffer));

	fd_set rfds;
	struct timeval tv;
	int retval;

    /* Watch stdin (fd 0) to see when it has input. */

    /* Wait up to two seconds. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	while(1)
	{
		FD_ZERO(&rfds);
		int fdnum=-1;
		for(int i=0;i<nsfd.size();i++)
		{
			FD_SET(nsfd[i],&rfds);
			if(fdnum<nsfd[i])
			{
				fdnum=nsfd[i];
			}

		}

		FD_SET(csfd,&rfds);
		if(fdnum<usfd)
		{
			fdnum=usfd;
		}
		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);
		if(retval>0)
		{
			for (int i = 0; i < nsfd.size(); ++i)
			{
				if(FD_ISSET(nsfd[i],&rfds))
				{
					//cout<<"yoyo\n";
					memset(buffer,'\0',sizeof(buffer));
					int r = recv(nsfd[i],buffer,sizeof(buffer),0);
					strcat(buffer,": from s1");
					if(r>0)
					{
						send(nsfd[i],buffer,sizeof(buffer),0);
					}else if(r==0)
					{
						cout<<"connection closed "<<nsfd[i]<<"\n";
						nsfd.erase(nsfd.begin()+i);
						if(nsfd.size()==0)
							exit(1);
					}else
					{
						perror("recv error");
						exit(0);
					}
					break;
				}

			}

			if(FD_ISSET(csfd,&rfds))
			{
				//cout<<"usfd call\n";
				nsfd.push_back(recv_file_descriptor(csfd));

			}
		}else if(retval<0)
		{
			perror("select error");
			exit(1);
		}
		
	}

	return 0;
}
