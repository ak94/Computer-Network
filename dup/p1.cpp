#include "iostream"
#include "cstdlib"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"
#include "signal.h"

using namespace std;

int c;

void signal_handler(int signum)
{
	
	int n;
	
	if(cin>>n)
	{
		fflush(stdin);
		cout<<"P1:"<<n<<"\n";
		fflush(stdout);
		signal(signum,signal_handler);
		kill(c,SIGUSR1);
	}
	else
	{
		exit(1);
	}

}

int main(int argc, char const *argv[])
{
	int fd;
	signal(SIGUSR1,signal_handler);

	fd = open("file.txt",O_RDONLY);


	if (fd==-1)
	{
		perror("File open error");
		exit(1);
	}

	if(dup2(fd,0)==-1)
	{
		perror("dup2() error");
		exit(1);
	}

	c = fork();

	cout<<"val "<<c<<endl;

	if(c>0)
	{
		//signal(SIGUSR1,signal_handler);
		//cout<<"Parent";
		int k=1;
		while((k=sleep(k))!=0);
		
		kill(getpid(),SIGUSR1);


	}else if(c==0)
	{

	//	cout<<"child";
		execl("./p2","p2",NULL);

	}else
	{
		perror("Fork error");
		exit(1);
	}

	while(1);

	return 0;
}
