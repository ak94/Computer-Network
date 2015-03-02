#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
int got_usr1=1;
int shmid,*pid,*x,shid;

int main()
{
	int i=0,fd1,fd2;
	char s[300];
	pid=(int*)malloc(4*sizeof(int));
	int key=ftok("servers.c",'R');
	shmid=shmget(key,30,0666|IPC_CREAT);
	pid=(int*)shmat(shmid,(void*)0,0);
	pid[0]=getpid();	

	int tag=1;
	while(tag)
	{
		if((pid[0]!=-1)&&(pid[1]!=-1)&&(pid[2]!=-1))
		tag=0;
	}
	mknod("C1_1", S_IFIFO | 0666, 0);
	mknod("C1_2", S_IFIFO | 0666, 0);
	
	int c=fork();
	if(c>0)
	{
		fd1= open("C1_1",O_WRONLY);
		printf("CONNECTED\n");
		while(1)
		{
			gets(s);
			write(fd1,s,strlen(s));	
			kill(pid[3],SIGUSR1);
		}
	}
	else
	{
		fd2= open("C1_2",O_RDONLY);
		printf("CONNECTED\n");
		while(1)
		{
			if(read(fd2,s,300)!=-1)
			printf("...%s\n",s);
		}
	}
}
