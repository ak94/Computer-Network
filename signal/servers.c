#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <poll.h>
#include <sys/shm.h>

int shmid,*pid,shid,*x;

int num,fd1[3],fd2[3];

char s[300];

void sigusr1_handler(int signo,siginfo_t *siginfo, void *context)
{
	int i,j;
	for(i=0;i<3;i++)
	{
		if(pid[i]==(int)siginfo->si_pid)
		{
			if(read(fd1[i],s,300)!=-1)
			{
				for(j=0;j<3;j++)
				    if(i!=j)
					write(fd2[j],s,strlen(s));
			}
			break;  
		}
	}
}
int main(void)
{
    
    int i,j;

    pid=(int*)malloc(4*sizeof(int));

    int key=ftok("servers.c",'R');
    shmid=shmget(key,30,0666|IPC_CREAT);

    pid=(int*)shmat(shmid,(void*)0,0);

    pid[3]=getpid();
 
    char *name1[3]={"C1_1","C2_1","C3_1"};
    char *name2[3]={"C1_2","C2_2","C3_2"};
    
    for(i=0;i<3;i++)
    {
	
	mknod(name1[i], S_IFIFO | 0666, 0);
	mknod(name2[i], S_IFIFO | 0666, 0);
	fd1[i] = open(name1[i], O_RDONLY);
	fd2[i] = open(name2[i], O_WRONLY);
	printf("C%d CONNECTED\n",i+1);
    }
    printf("CONNECTED\n");

	struct sigaction sa;
	sa.sa_sigaction = &sigusr1_handler;
    	sa.sa_flags = SA_SIGINFO;
    	sigemptyset(&sa.sa_mask);
    	
	sigaction(SIGUSR1,&sa,NULL);
   
    while(1);

    return 0;
}
