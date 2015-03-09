#include "bits/stdc++.h"
#include "unistd.h"
#include "sys/socket.h"
#include "sys/types.h"
#include <arpa/inet.h> 

#define backlog 3

using namespace std;
int main(int argc, char const *argv[])
{


	int nsfd;
	int lsfd;
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

	nsfd = accept(lsfd,(struct sockaddr *)&client,(socklen_t *)&c_len);

	send(nsfd,"hi",3,0);

	int c = nsfd;

	send(c,"yo",3,0);

	close(nsfd);
	return 0;

}