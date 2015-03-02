#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

void sighandler(int signo){
	int c=getppid();
	char s[40];
	char k;

	cout<<"P2 ";
	
	if(cin.getline(s,40,'\n')){
	fflush(stdin);
	cout<<s<<endl;
	fflush(stdout);
	signal(signo,sighandler);
	kill(c,SIGUSR1);
	
	}
}



int main()
{

	signal(SIGUSR1,sighandler);
	kill(getppid(),SIGUSR1);
	while(1);

}