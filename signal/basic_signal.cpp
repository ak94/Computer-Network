#include "iostream"
#include "csignal"
#include "cstdlib"
#include "unistd.h"

using namespace std;

void signal_handler(int signum)
{
	cout<<"int:"<<signum<<"\n";
	exit(signum);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT,signal_handler);
	while(1)
	{
		cout<<"do somthing\n";
		sleep(1);

	}
	return 0;
}