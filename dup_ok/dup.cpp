#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <wait.h>

using namespace std;

int c;

void sighandler(int signo){
	
	char s[40];
	
	cout<<"P1 ";
	if(cin.getline(s,40,'\n'))
	{
	fflush(stdin);
	cout<<s<<endl;
	fflush(stdout);
	signal(signo,sighandler);
	kill(c,SIGUSR1);
	
	}
	else
		kill(getpid(),SIGUSR2);
}

void sighandler2(int signo){
	exit(1);
}
int main(){
	
	signal(SIGUSR1,sighandler);
	signal(SIGUSR2,sighandler2);

	int pfd[2];

	int fd;
	fd=open("testfile.txt",O_RDONLY);

	dup2(fd,0);
	close(fd);

	c=fork();
	
	if(c>0){

		int v=1;
		while((v=sleep(v))!=0);
		
		kill(getpid(),SIGUSR1);		
		while(1);
	

	}
	else{

		execlp("./p2","p2",NULL);
	}
}