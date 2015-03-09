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

	nsfd = accept(sfd,(struct sockaddr *)&client,(socklen_t *)&clilen);
	if(nsfd<0)
	{
		perror("accept error");
		exit(1);
	}

	send(nsfd,"resource is going to serve you.\n",40,0);

	int usfd;
	struct sockaddr_un sender;
	
	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	sender={0};
	sender.sun_family = AF_UNIX;
	strncpy(sender.sun_path,path,sizeof(sender.sun_path)-1);

	if(connect(usfd,(struct sockaddr *)&sender,sizeof(struct sockaddr_un))<0)
	{
		perror("connect error");
		exit(1);
	}

	if(send_file_descriptor(usfd,nsfd)<0)
	{
		perror("sendmsg error");
		exit(0);
	}
	cout<<"nsfd sent\n";
	close(nsfd);
	while(1);

	return 0;
}
