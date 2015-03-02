#include "bits/stdc++.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "cstring"
#define backlog 2
using namespace std;

int n=2;
std::vector<int> lsfd;
std::vector<struct sockaddr_in> server;
std::vector<vector<int> > s;
std::vector<int> limit,ports;
bool rukjao=false;
//thread tt;

void sig_int(int);
void running();

void running()
{
	cout<<"running\n";
	sockaddr_in client;
	int c_len;
	fd_set rfds;
	//signal(SIGUSR1,sig_int);
	struct timeval tv;

	tv.tv_sec = 30;
	tv.tv_usec =0;

	while(1)
	{
		//while(rukjao);

		FD_ZERO(&rfds);
		int fdnum=-1;

		for (int i = 0; i < lsfd.size(); ++i)
		{
			FD_SET(lsfd[i],&rfds);
			if(fdnum<lsfd[i])
			{
				fdnum=lsfd[i];
			}
		}
		
		for (int i = 0; i < s.size(); ++i)
		{
			for(int j=0;j<s[i].size();j++)
			{
				FD_SET(s[i][j],&rfds);
				if(fdnum<s[i][j])
				{
					fdnum=s[i][j];
				}
			}
			
		}

		int retval = select(fdnum+1,&rfds,NULL,NULL,&tv);
		//cout<<"retval "<<retval;
		if(retval>0)
		{
			cout<<"yo\n";
			char buffer[2000];
			memset(buffer,'\0',sizeof(buffer));
			for (int i = 0; i < s.size(); ++i)
			{
				for(int j=0;j<s[i].size();j++)
				{
					if(FD_ISSET(s[i][j],&rfds))
					{
						recv(s[i][j],buffer,sizeof(buffer),0);
						cout<<"echo to "<<s[i][j]<<"\n";
						send(s[i][j],buffer,sizeof(buffer),0);
						close(s[i][j]);
						s[i].erase(s[i].begin()+j);
					}
				}
				
			}
			//cout<<"retval "<<retval<<"\n";
			for (int i = 0; i < lsfd.size(); ++i)
			{
				//cout<<"i "<<i;
				if(FD_ISSET(lsfd[i],&rfds))
				{
					//cout<<"connection request for lsfd "<<lsfd[i]<<endl;

					if(s[i].size()>=limit[i])
					{
						//cout<<"limit reached";
					}else
					{
						s[i].push_back(accept(lsfd[i],(struct sockaddr *)&client,(socklen_t *)&c_len));
						cout<<"accepted\n";
					}
				}
			}

			
		}else if(retval<0)
		{
			perror("select()");
			exit(1);
		}
	}

}
void sig_int(int signo,siginfo_t *siginfo, void *contexto)
{
	
	char x;
	cout<<"quit(q) or add(a) or delete(d)";
	cin>>x;
	if(x=='q')
	{
		exit(0);
	}
	else{
		int p;
		cout<<"new port no?";
		cin>>p;
		bool found=false;

		for(int i=0;i<ports.size();i++)
		{
			if(p==ports[i])
			{
				cout<<"port exist";
				found=true;
				break;
			}
		}

		if(!found)
		{
			//rukjao=true;
			ports.push_back(p);
			lsfd.push_back(socket(AF_INET,SOCK_STREAM,0));
			server.resize(server.size()+1);
			server[server.size()-1].sin_family = AF_INET;
			server[server.size()-1].sin_addr.s_addr=INADDR_ANY;
			server[server.size()-1].sin_port = htons(p);
			if(bind(lsfd.back(),(struct sockaddr *)&server[server.size()-1],sizeof(server[server.size()-1]))<0)
			{
				perror("bind error");
				exit(1);

			}

			cout<<"bind done\n";

			listen(lsfd.back(),backlog);
			cout<<"limit for this port\n";
			cin>>p;
			limit.push_back(p);
			s.resize(s.size()+1);
			
			//rukjao=false;
		}
	}
	running();
}

int main(int argc, char const *argv[])
{
	struct sigaction sa;
	sa.sa_sigaction = &sig_int;
    	sa.sa_flags = SA_SIGINFO;
    	sigemptyset(&sa.sa_mask);
    	
	sigaction(SIGINT,&sa,NULL);
	
	lsfd.clear();
	lsfd.resize(n);
	server.clear();
	server.resize(n);
	s.clear();
	s.resize(n);
	limit.clear();
	limit.resize(n);
	limit[0]=2;
	limit[1]=1;
	//thread (lol).detach();
	ports.clear();
	ports.push_back(8888);
	ports.push_back(8889);
	for(int i=0;i<n;i++)
	{
		lsfd[i]=socket(AF_INET,SOCK_STREAM,0);
		server[i].sin_family = AF_INET;
		server[i].sin_addr.s_addr=INADDR_ANY;
		server[i].sin_port = htons(ports[i]);
		if(bind(lsfd[i],(struct sockaddr *)&server[i],sizeof(server[i]))<0)
		{
			perror("bind error");
			exit(1);

		}

		cout<<"bind done\n";

		listen(lsfd[i],backlog);
	}

	running();
	return 0;
}