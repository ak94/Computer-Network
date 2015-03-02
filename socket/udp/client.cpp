#include "bits/stdc++.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "unistd.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int usfd;
	struct sockaddr_in server,from;
	int len;
	usfd = socket(AF_INET,SOCK_DGRAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(8888);

	while(1)
	{
		char buffer[2000];
		memset(buffer,'\0',sizeof(buffer));
		cout<<"enter msg\n";
		cin>>buffer;
		sendto(usfd,buffer,sizeof(buffer),0,(const struct sockaddr *)&server,sizeof(server));
		int r = recvfrom(usfd,buffer,sizeof(buffer),0,NULL,NULL);
		if(r>0)
			cout<<buffer<<"\n";
		else
		{
			perror("ohh");
		}
	}
	return 0;
}