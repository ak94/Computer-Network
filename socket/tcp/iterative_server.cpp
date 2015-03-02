#include "bits/stdc++.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include <arpa/inet.h> 

#define backlog 3

using namespace std;

std::map<int,bool> map_client;
int lsfd;
void serve_client(int nsfd)
{
	cout<<"serving client with nsfd "<<nsfd<<"\n";
	char buffer[2000];
	memset(buffer,'\0',sizeof(buffer));
	while(1)
	{
		ssize_t r = recv(nsfd,buffer,sizeof(buffer),0);
		if(r<0)
		{
			perror("recv error");
			break;
		}else if(r==0)
		{
			cout<<"client closed connection\n";
			break;
		}
		else
		{
			for(std::map<int,bool>::iterator i = map_client.begin(); i != map_client.end(); ++i)
			{
				if(i->first==nsfd)
					continue;
				else if(i->second==true)
					send(i->first,buffer,sizeof(buffer),0);
			}
		}
	}
	close(nsfd);
	map_client[nsfd]=false;
	bool active=false;
	for (std::map<int,bool>::iterator i = map_client.begin(); i != map_client.end(); ++i)
	{
		if(i->second==true)
		{
			active=true;
			break;
		}
	}

	if(!active)
	{
		cout<<"All clients gone\n";
		close(lsfd);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{

	std::thread t[3];
	map_client.clear();
	int nsfd;
	struct sockaddr_in server,client;
	int c_len;
	lsfd = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port = htons(8888);

	if(bind(lsfd,(struct sockaddr *)&server,sizeof(server))<0)
	{
		perror("bind failed");
	}
	cout<<"bind done\n";

	listen(lsfd,backlog);
	int index=0;
	while(1)
	{
		cout<<"waiting for accept\n";
		nsfd = accept(lsfd,(struct sockaddr *)&client,(socklen_t *)&c_len);
		if(nsfd<0)
			perror("accept failed");
		else
		{
			if(map_client.find(nsfd)==map_client.end()||map_client[nsfd]==false)
			{
				map_client[nsfd]=true;
			}
			t[index]=thread(serve_client,nsfd);
				//t[index].join();
			index++;
			//bool active=false;


		}

	}

	
	return 0;
}