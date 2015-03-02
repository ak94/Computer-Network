#include "sys/stat.h"
#include "sys/types.h"
#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"

#define path "1234asd"
#define path2 "1234asd1"
#define path3	"client2_rec"
#define path4	"client2_send"

#define maxbuf 1024

int fd[4];


void *send(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		memset(buffer,'\0',maxbuf);

		scanf("%s",buffer);
		if(strcmp(buffer,"exit")==0)
			break;
		write(fd[0],buffer,maxbuf);
	}
}

void *receive(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		read(fd[1],buffer,maxbuf);
		printf("%s\n", buffer);
		write(fd[2],buffer,maxbuf);
	}
}

void *c2_send(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		memset(buffer,'\0',maxbuf);

		scanf("%s",buffer);
		if(strcmp(buffer,"exit")==0)
			break;
		write(fd[2],buffer,maxbuf);
	}
}

void *c2_rec(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		read(fd[3],buffer,maxbuf);
		printf("%s\n", buffer);
		write(fd[0],buffer,maxbuf);
	}
}

int main(int argc, char const *argv[])
{
	mkfifo(path,0666);
	mkfifo(path2,0666);
	mkfifo(path3,0666);
	mkfifo(path4,0666);
	
	fd[0] = open(path,O_WRONLY);
	fd[1] = open(path2,O_RDONLY);
	fd[2] = open(path3,O_WRONLY);
	fd[3] = open(path4,O_RDONLY);

	

	pthread_t t[4];

	pthread_create(&t[0],NULL,send,NULL);
	pthread_create(&t[1],NULL,receive,NULL);
	pthread_create(&t[2],NULL,c2_rec,NULL);
	pthread_create(&t[3],NULL,c2_send,NULL);

	pthread_join(t[0],NULL);

	close(fd);
	unlink(fd);

	return 0;
}