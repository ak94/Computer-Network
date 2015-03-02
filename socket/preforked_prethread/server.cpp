#include "bits/stdc++.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/wait.h"
#define backlog 2
using namespace std;
vector<int> child_id;

int create_child(int);
void child_main(int);
void child_server(int);
void sig_hand(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
	{
		cout<<"child "<<pid<<" terminated \n";
	}

	return;
}
int lsfd;
struct sockaddr_in server;
int main(int argc, char const *argv[])
{
	int prefork,prethread;
	cout<<"No of prefork : ";
	cin>>prefork;
	cout<<"\nNo of prethread : ";
	cin>>prethread;
	child_id.clear();
	child_id.resize(prefork);

	//signal(SIGCHLD,sig_hand);

	
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

	for (int i = 0; i < prefork; ++i)
	{
		int c;
		c=fork();
		if(c==0)
		{
			child_main(prethread);
		}
	}

	close(lsfd);

	while(1);
	
}

int create_child(int m)
{
	int c;
	c=fork();
	if(c>0)
	{
		return c;
	}else
	{
		child_main(m);
	}
}

void child_server(int server_no)
{
	cout<<"child server "<<server_no<<"started \n";
	
	int c_len;
	struct sockaddr_in client;
	fd_set rfds;
	
	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec =0;
	std::vector<int> nsfd;
	nsfd.clear();
	int retval;

	while(1)
	{
		FD_ZERO(&rfds);
		retval=0;
		int fdnum=-1;
		for (int i = 0; i < nsfd.size(); ++i)
		{
			FD_SET(nsfd[i],&rfds);
			if(fdnum<nsfd[i])
			{
				fdnum=nsfd[i];
			}
		}

		FD_SET(lsfd,&rfds);
		if(fdnum<lsfd)
		{
			fdnum=lsfd;
		}

		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);
		//cout<<"retval "<<retval;
		if(retval>0)
		{
			char buffer[2000];
			memset(buffer,'\0',sizeof(buffer));
			if(FD_ISSET(lsfd,&rfds))
			{
				//cout<<"Client request came\n";
				nsfd.push_back(accept(lsfd,(struct sockaddr *)&client,(socklen_t *)&c_len));
				cout<<"accept complete "<<nsfd.back()<<"\n";
			}
			for (int i = 0; i < nsfd.size(); ++i)
			{
				
				if(FD_ISSET(nsfd[i],&rfds))
				{
					recv(nsfd[i],buffer,sizeof(buffer),0);
					cout<<"echo to nsfd no"<<nsfd[i]<<"\n";
					send(nsfd[i],buffer,sizeof(buffer),0);
					//close(nsfd[i]);
					//nsfd.erase(nsfd.begin()+i);
				}
			}

			
		}else if(retval<0)
		{
			perror("select()");
			exit(1);
		}

		//cout<<"select timeout.... \n";
	}
	cout<<"out of loop "<<server_no<<"\n";
}

void child_main(int m)
{
	thread t[m];
	cout<<"child main\n";
	for (int i = 0; i < m; ++i)
	{
		t[i]=thread(child_server,i+1);
		
	}

	for(int i=0;i<m;i++)
		t[i].join();
	
}