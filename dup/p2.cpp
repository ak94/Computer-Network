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

void signal_handler(int signum)
{
	int n;
	
	if(cin>>n)
	{
		fflush(stdin);
		cout<<"P2:"<<n<<"\n";
		fflush(stdout);
		signal(signum,signal_handler);
		kill(getppid(),SIGUSR1);
	}
	else
	{
		exit(1);
	}
}
int main(int argc, char const *argv[])
{
	signal(SIGUSR1,signal_handler);
	/*int k =1;
	while((k=sleep(k))!=0);*/
	kill(getppid(),SIGUSR1);
	while(1);

	return 0;
}