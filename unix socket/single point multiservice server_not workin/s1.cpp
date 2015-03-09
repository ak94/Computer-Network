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

static int recv_fd(int socket) /* Socket from which the file descriptor is read */
{

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
	cout<<"Starting s1\n";
	int usfd;
	dup2(2,usfd);
	close(2);

	fd_set rfds;
	
	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec =0;

	std::vector<int> nsfd;
	nsfd.clear();

	while(1)
	{
		cout<<"asdf";
		FD_ZERO(&rfds);
		int retval = 0;
		int fdnum = -1;
		for (int i = 0; i < nsfd.size(); ++i)
		{
			FD_SET(nsfd[i],&rfds);
			if(fdnum<nsfd[i])
			{
				fdnum=nsfd[i];
			}
		}
		FD_SET(usfd,&rfds);
		if(fdnum<usfd)
		{
			fdnum=usfd;
		}

		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);
		if(retval>0)
		{
			cout<<"retval";
			for(int i=0;i<nsfd.size();i++)
			{
				if(FD_ISSET(nsfd[i],&rfds))
				{
					char buffer[2000];
					memset(buffer,'\0',sizeof(buffer));

					recv(nsfd[i],buffer,sizeof(buffer),0);
					strcat(buffer," from s1");
					send(nsfd[i],buffer,sizeof(buffer),0);
					break;
				}
			}

			if(FD_ISSET(usfd,&rfds))
			{
				nsfd.push_back(recv_fd(usfd));
			}
		}else if(retval < 0)
		{

			perror("s1 select ");
		}
		return 0;
	}
}