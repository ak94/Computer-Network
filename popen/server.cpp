#include "bits/stdc++.h"
#include "stdio.h"
#include "unistd.h"

using namespace std;

int main(int argc, char const *argv[])
{
	FILE *fp = popen("./r1","r");
	int fd = fileno(fp);
	while(1)
	{
		char buffer[20];
		memset(buffer,'\0',sizeof(buffer));
		int r = read(fd,buffer,sizeof(buffer));
		if(r>0)
		{
			cout<<buffer<<"\n";
		}else if(r==0)
		{
			cout<<"pipe close\n";
			pclose(fp);
			return 0;
		}
		else
		{
			perror("recv error");
			return 0;
		}
	}


	return 0;
}