#include "bits/stdc++.h"
#include "unistd.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int pid;
	cout<<"This is main\n";
	int i=0;
	pid = fork();

	if(pid==0)
	{
		cout<<"This is child\n";
		i++;
		cout<<"Value of i in child process = "<<i<<"\n";
	}else if(pid>0)
	{
		cout<<"This is parent\n";
		i++;
		cout<<"Value of i in parent process = "<<i<<"\n";
	}else
	{
		cout<<"Fork Error\n";
	}

	return 0;
}