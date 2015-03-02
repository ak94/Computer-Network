#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "stdio.h"

#define path3	"client2_rec"
#define path4	"client2_send"

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
	
	mkfifo(path3,0666);
	mkfifo(path4,0666);

	pthread_t t[2];


	fd[0] = open(path3,O_RDONLY);
	fd[1] = open(path4,O_WRONLY);

	pthread_create(&t[0],NULL,receive,NULL);
	pthread_create(&t[1],NULL,send,NULL);
	pthread_join(t[0],NULL);

	close(fd);
	return 0;
};