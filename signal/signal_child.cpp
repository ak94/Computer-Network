#include "csignal"
#include "iostream"
#include "unistd.h"
#include "cstdlib"
#include "errno.h"
#include "stdio.h"
using namespace std;


void signal_handler(int signum)
{
	cout<<"int: "<<signum<<"\n";
	//signal(signum,signal_handler);
	
}



int main(int argc, char const *argv[])
{
	signal(SIGUSR1,signal_handler);
	//signal(SIGUSR2,signal_handler2);
	int c = fork();
	if(c>0)
	{
		signal(SIGUSR1,signal_handler);
		int k=1;
		while((k=sleep(k))!=0);
		cout<<"I am parent\n";
		kill(c,SIGUSR1);
		//wait();

	}else if(c==0)
	{
			//signal(SIGUSR1,signal_handler);
			signal(SIGUSR1,signal_handler);
			int k=1;
			while((k=sleep(k))!=0);
			cout<<"I am child\n";
			kill(getppid(),SIGUSR1);
			
	}else
	{
		perror(NULL);
		exit(0);
	}
	return 0;
}
