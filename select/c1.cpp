#include "iostream"
#include "sys/stat.h"
#include "sys/types.h"
#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "string.h"
#include "poll.h"
#include "cstdlib"

#define fifo_a "fifo_a"
#define fifo_b "fifo_b"

#define maxbuf 1024

using namespace std;

int main(int argc, char const *argv[])
{
	int ffd[2];
	char buffer[maxbuf];
	mkfifo(fifo_a,0666);
 	mkfifo(fifo_b,0666);

 	ffd[0]=open(fifo_a,O_RDONLY);
 	ffd[1]=open(fifo_b,O_WRONLY);

 	int c = fork();
 	if(c>0)
 	{
 		while(1)
 		{
 			memset(buffer,'\0',maxbuf);
 			//cout<<"write msg:\n";
 			scanf("%s",buffer);
 			if(buffer[0]!='\0')
 			write(ffd[1],buffer,maxbuf);
 		}
 	}else if(c==0)
 	{
 		while(1)
 		{
 			memset(buffer,'\0',maxbuf);
 			read(ffd[0],buffer,maxbuf);
 			//cout<<"msg received:\n";
 			if(buffer[0]!='\0')
 			cout<<buffer<<"\n";
 		}
 	}else
 	{
 		perror("fork error");
 		exit(1);
 	}

	return 0;
}