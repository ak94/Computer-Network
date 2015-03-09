#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"
#include "sys/wait.h"
using namespace std;

#define backlog 3
#define path "/tmp/unix_socket"
std::vector<bool> go;
int sfd,nsfd;
int csfd;
struct sockaddr_in server;
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
void run(int index)
{
	
	while(1)
	{	
		struct sockaddr_in client;
		socklen_t clilen;
		cout<<"thread "<<index+1<<" started\n";
		while(!go[index]);

		int nsfd = accept(sfd,(struct sockaddr *)&client,(socklen_t *)&clilen);
		go[index]=false;
		go[index+1]=true;
		cout<<"thread "<<index+1<<" accept\n";
		while(1)
		{
			char buffer[200];
			memset(buffer,'\0',sizeof(buffer));

			int r=recv(nsfd,buffer,sizeof(buffer),0);
			if(r>0)
				{	
					char kk[20];
					sprintf(kk,"from s2 thread %d",index+1);
					strcat(buffer,kk);
					send(nsfd,buffer,sizeof(buffer),0);
				}
			else if(r==0)
			{
				cout<<"connection closed on thread"<<index<<"\n";
				close(nsfd);
				go[index]=false;
				break;
			}else
			{
				perror("recv error");
				exit(1);
			}	
		}
	}
}

void special(int index)
{
	cout<<"special waiting\n";
	while(!go[index]);

	send_file_descriptor(csfd,sfd);
	cout<<"fd sent from s2\n";
	go[index]=false;
	close(sfd);
}
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

void incoming_fd()
{
	while(1)
	{

	//read_fd(csfd,&nsfd);
		sfd = recv_file_descriptor(csfd);
		cout<<"fd recv from s1\n";
		if(!go[0])
		{
			cout<<"sending to thread 1\n";
			go[0]=true;
		}
		else
		{
			cout<<"all thread busy\n";
		}
	}
}
int main(int argc, char const *argv[])
{
	go.clear();
	go.resize(3,false);
	
	int usfd;
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
	cout<<"bind usfd success r1\n";
	listen(usfd,backlog);
	//signal(SIGCHLD,sig_chld);
	
	clilen =sizeof(struct sockaddr_un);
	csfd = accept(usfd,(struct sockaddr *)&client,&clilen);

	if(csfd<0)
	{
		perror("accept error");
		exit(1);
	}


	cout<<"connection ufd established\n";

	thread t[4];
	for (int i = 0; i < 2; ++i)
	{
		t[i]=thread(run,i);
	}

	t[2]=thread(special,2);
	t[3]=thread(incoming_fd);
	for (int i = 0; i < 4; ++i)
	{
		t[i].join();
	}

	return 0;
}