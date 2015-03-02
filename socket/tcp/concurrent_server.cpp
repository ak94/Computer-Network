#include "bits/stdc++.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/wait.h"
#define backlog 3
using namespace std;

void service(int nsfd)
{
	cout<<"Servicing client "<<nsfd<<"\n";
	
	char buffer[2000];
	memset(buffer,'\0',sizeof(buffer));
	while(1)
	{
		int r = recv(nsfd,buffer,sizeof(buffer),0);
		if(r>0)
		{
			send(nsfd,buffer,sizeof(buffer),0);
		}else if(r==0)
		{
			cout<<"connection closed "<<nsfd<<"\n";
			return;
		}else
		{
			perror("recv error");
			return;
		}
	}
	return;
}

void sighandler(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
		cout<<"child "<<pid<<" terminated\n";
	return;
}

int main(int argc, char const *argv[])
{
	
	int lsfd,nsfd;
	struct sockaddr_in server,client;
	int c_len;

	lsfd = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port = htons(8888);
	if(bind(lsfd,(struct sockaddr *)&server,sizeof(server))<0)
	{
		perror("bind error");
		exit(1);

	}

	cout<<"bind done\n";

	listen(lsfd,backlog);
	signal(SIGCHLD,sighandler);

	while(1)
	{
		nsfd = accept(lsfd,(struct sockaddr *)&client,(socklen_t *)&c_len);
		int c = fork();
		if(c>0)
		{
			close(nsfd);
		}else if(c==0)
		{
			close(lsfd);
			service(nsfd);
			close(nsfd);
			exit(0);
		}else
		{
			perror("fork error");

		}

	}

	return 0;
}