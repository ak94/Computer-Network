#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "stdio.h"

#define path "1234asd" 
#define path2 "1234asd1"
#define maxbuf 1024


int fd[2];

void *receive(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		read(fd[0],buffer,maxbuf);
		printf("%s\n", buffer);
	}
}

void *send(void *x)
{
	char buffer[maxbuf];
	while(1)
	{
		memset(buffer,'\0',maxbuf);

		scanf("%s",buffer);
		if(strcmp(buffer,"exit")==0)
			break;
		write(fd[1],buffer,maxbuf);
	}
}

int main(int argc, char const *argv[])
{
	
	mkfifo(path,0666);
	mkfifo(path2,0666);

	pthread_t t[2];


	fd[0] = open(path,O_RDONLY);
	fd[1] = open(path2,O_WRONLY);

	pthread_create(&t[0],NULL,receive,NULL);
	pthread_create(&t[1],NULL,send,NULL);
	pthread_join(t[0],NULL);

	close(fd);
	return 0;
};