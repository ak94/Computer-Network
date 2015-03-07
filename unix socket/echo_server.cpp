#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"
#include "sys/wait.h"
using namespace std;

#define path "/tmp/unix_socket_file"
#define backlog 3
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
	{
		cout<<"child "<<pid<<" terminated \n";
	}

	return;
}
int main(int argc, char const *argv[])
{
	int usfd,csfd;
	struct sockaddr_un server,client;
	socklen_t clilen;

	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	unlink(path);
	memset(&server,0,sizeof(struct sockaddr_un));

	server.sun_family=AF_UNIX;
	strncpy(server.sun_path,path,sizeof(server.sun_path)-1);

	if(bind(usfd,(struct sockaddr *)&server,sizeof(struct sockaddr_un))<0)
	{
		perror("bind error");
		exit(1);
	}
	cout<<"bind success\n";
	listen(usfd,backlog);
	signal(SIGCHLD,sig_chld);
	while(1)
	{
		clilen =sizeof(struct sockaddr_un);
		csfd = accept(usfd,(struct sockaddr *)&client,&clilen);
		if(csfd==-1)
		{
			perror("accept error");

		}

		pid_t c = fork();
		if(c>0)
		{
			close(csfd);
		}else
		{
			close(usfd);
			char buffer[2000];

			while(1)
			{
				memset(buffer,'\0',sizeof(buffer));
				recv(csfd,buffer,sizeof(buffer),0);
				strcat(buffer," thankyou");
				cout<<"echo to nsfd no"<<csfd<<"\n";
				send(csfd,buffer,sizeof(buffer),0);
			}
		}	

	}

	return 0;
}
