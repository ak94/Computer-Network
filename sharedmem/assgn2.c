#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

int shmid1;//for resources

int shmid2;//for pid's

int *s,*p,x,y;

int got_usr2;

void sigusr2_handler(int signo)
{
	got_usr2=1;
}

int main()
{
	s=(int*)malloc(2*sizeof(int));
	p=(int*)malloc(2*sizeof(int));

	int key1=ftok("assgn1.c",'R');
	int key2=ftok("assgn2.c",'R');

	shmid1=shmget(key1,30,0666|IPC_CREAT);
	shmid2=shmget(key2,30,0666|IPC_CREAT);

	s=shmat(shmid1,(void*)0,0);
	p=shmat(shmid2,(void*)0,0);

	p[1]=getpid();
	
	signal(SIGUSR2,sigusr2_handler);
	//kill(p[0],SIGUSR1);
	while(x<=10)
	{
		while(!got_usr2);
		got_usr2=0;
		y=s[1];
		printf("Y = %d\t",y);
		x=y+1;
		printf("X = %d\n",x);
		s[0]=x;
		kill(p[0],SIGUSR1);
	}
	
}
