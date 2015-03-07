#include "bits/stdc++.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/un.h"

using namespace std;

#define path "/tmp/unix_socket_file"


int main(int argc, char const *argv[])
{
	int usfd,csfd;
	struct sockaddr_un server,client;
	socklen_t clilen;

	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	//unlink(path);
	memset(&server,0,sizeof(struct sockaddr_un));

	server.sun_family=AF_UNIX;
	strncpy(server.sun_path,path,sizeof(server.sun_path)-1);

	if(connect(usfd,(struct sockaddr *)&server,sizeof(struct sockaddr_un))<0)
	{
		perror("connect error");
		exit(1);
	}
	char buffer[2000];
	while(1)
	{
		memset(buffer,'\0',sizeof(buffer));
		cin>>buffer;
		send(usfd,buffer,sizeof(buffer),0);
		recv(usfd,buffer,sizeof(buffer),0);
		cout<<"from server : "<<buffer<<"\n";
	}

	return 0;
}
