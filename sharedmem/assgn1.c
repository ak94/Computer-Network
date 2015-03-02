#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

int shmid1;//for resources
int shmid2;//for pid's

int *s,*p,x,y;

int got_usr1;

void sigusr1_handler(int signo)
{
	got_usr1=1;
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

	p[0]=getpid();
	s[0]=s[1]=0;
	p[1]=-1;

	signal(SIGUSR1,sigusr1_handler);
	got_usr1=1;
	/*sigset_t mask,oldmask;
	sigemptyset(&mask);
	sigemptyset(&oldmask);*/
	while(x<=10)
	{
		while(!got_usr1);
		/*sigprocmask(SIG_BLOCK, &mask, &oldmask);
   		sigsuspend (&oldmask);
 		sigprocmask(SIG_UNBLOCK, &mask, NULL);*/
		x=s[0];
		got_usr1=0;
		printf("X = %d\t",x);
		y=x+1;
		printf("Y = %d\n",y);
		s[1]=y;
		
		while(p[1]==-1);
		kill(p[1],SIGUSR2);	
	}
	return 0;
}
