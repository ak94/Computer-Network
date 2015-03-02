/**
 * reading from a file
 */
#include "iostream"
#include "cstdlib"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "errno.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int fd,fd1;
	
	fd = open("file.txt",O_RDONLY);
	//fd1 = open("file2.txt",O_)
	if (fd==-1)
	{
		perror("File open error");
		exit(1);
	}
	close(0);
	if(fcntl(fd,F_DUPFD,0)==-1)
	{
		perror("fcntl() error");
		exit(1);
	}
	string s;
	while(cin>>s)
	{
		cout<<s<<" ";
	}

	return 0;
}