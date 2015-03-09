#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"
#include "sys/wait.h"

using namespace std;

#define backlog 5
#define path "/tmp/unix_socket"
static int send_fd(int socket,int fd_to_send)
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
	int sfd,nsfd;
	struct sockaddr_in server,client;
	socklen_t clilen;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	server = {0};
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port = htons(8888);

	if(bind(sfd,(struct sockaddr *)&server,sizeof(server))<0)
	{
		perror("bind sfd error");
		exit(0);
	}
	cout<<"bind sfd success\n";
	listen(sfd,backlog);

	/*fd_set rfds;
	
	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec =0;
	int retval;
	map<int,int> usfd;
	usfd.clear();*/

	/*while(1)
	{
		FD_ZERO(&rfds);
		retval=0;
		int fdnum=-1;
		for (std::map<int,int>::iterator i = usfd.begin(); i != usfd.end(); ++i)
		{
			FD_SET(i->second,&rfds);
			if(fdnum<i->second)
			{
				fdnum=i->second;
			}
		}

		FD_SET(sfd,&rfds);
		if(fdnum<sfd)
		{
			fdnum=sfd;
		}

		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);
		//cout<<"retval "<<retval;
		if(retval>0)
		{
			char buffer[2000];
			memset(buffer,'\0',sizeof(buffer));

			if(FD_ISSET(sfd,&rfds))
			{
				nsfd=accept(sfd,(struct sockaddr *)&client,(socklen_t *)&clilen);
				send(nsfd,"service number",15,0);
				recv(nsfd,buffer,sizeof(buffer),0);
				cout<<buffer<<" yo\n";
				int id=atoi(buffer);
				
				if(usfd.find(id)==usfd.end())
				{	
					cout<<"not \n";
					int sockfd[2];
					socketpair(AF_UNIX,SOCK_STREAM,0,sockfd);
					int c = fork();
					if(c>0)
					{
						close(sockfd[0]);
						usfd[id]=sockfd[0];
						
					}else if(c==0)
					{
						close(sockfd[1]);
						dup2(sockfd[1],2);
						if(id==1)
						{

							execl("./s1","s1",NULL);
						}else
						{
							execl("./s2","s2",NULL);
						}
						
					}
				}else
				{
					cout<<"found\n";
					send_fd(usfd[id],nsfd);
					close(nsfd);
				}
			}

			for (std::map<int,int>::iterator i = usfd.begin(); i != usfd.end(); ++i)
			{
				
				if(FD_ISSET(i->second,&rfds))
				{
					usfd.erase(i);
					break;					
				}
			}

			
		}else if(retval<0)
		{
			perror("select() server");
			exit(1);
		}

	}*/
map<int,int> usfd;
	usfd.clear();
		while(1)
		{
			nsfd=accept(sfd,(struct sockaddr *)&client,(socklen_t *)&clilen);
			char buffer[2000];
			memset(buffer,'\0',sizeof(buffer));
			send(nsfd,"service number",15,0);
			recv(nsfd,buffer,sizeof(buffer),0);
			cout<<buffer<<" yo\n";
			int id=atoi(buffer);

			if(usfd.find(id)==usfd.end())
			{	
				cout<<"not \n";
				int sockfd[2];
				socketpair(AF_UNIX,SOCK_STREAM,0,sockfd);
				int c = fork();
				if(c>0)
				{
					close(sockfd[0]);
					usfd[id]=sockfd[0];

				}else if(c==0)
				{
					close(sockfd[1]);
					//dup2(sockfd[1],2);
					snprintf(arg1,sizeof(arg1),"%d",sockfd[0]);
					snprintf(argmode,sizeof(arg1),"%d",O_RDWR);
					if(id==1)
					{

						execl("./s1","s1",agr1,pathname,argmode,(char *)NULL);
					}else
					{
						execl("./s2","s2",agr1,pathname,argmode,(char *)NULL);
					}

				}

			}else
				{
					cout<<"found\n";
					send_fd(usfd[id],nsfd);
					close(nsfd);
				}
}

			return 0;
		}