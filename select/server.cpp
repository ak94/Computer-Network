#include <sys/select.h>
#include "bits/stdc++.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "errno.h"
#include "stdlib.h"
#include "fcntl.h"

using namespace std;

#define maxbuf 1024

#define fifo_a "fifo_a"
#define fifo_b "fifo_b"
#define fifo_c "fifo_c"
#define fifo_d "fifo_d"

int main(int argc, char const *argv[])
{
	fd_set rfds;
	
	char buffer[maxbuf];
	int retval;

	int ffd[4];

	mkfifo(fifo_a,0666);
	mkfifo(fifo_b,0666);
	mkfifo(fifo_c,0666);
	mkfifo(fifo_d,0666);

	ffd[0]=open(fifo_a,O_WRONLY);
	ffd[1]=open(fifo_b,O_RDONLY);
	ffd[2]=open(fifo_c,O_WRONLY);
	ffd[3]=open(fifo_d,O_RDONLY);

	cout<<"connected";

	FD_ZERO(&rfds);

	int fdnum=-1;
	for (int i = 0; i < 4; ++i)
	{
		FD_SET(ffd[i],&rfds);
		if(fdnum<ffd[i])
		{
			fdnum=ffd[i];
		}
	}

	struct timeval tv;

	tv.tv_sec = 2;
	tv.tv_usec =0;

	
	while(1)
	{
		FD_ZERO(&rfds);

		int fdnum=-1;

		for (int i = 0; i < 4; ++i)
		{
			FD_SET(ffd[i],&rfds);
			if(fdnum<ffd[i])
			{
				fdnum=ffd[i];
			}
		}

		retval = select(fdnum+1,&rfds,NULL,NULL,&tv);

		if(retval>0)
		{
			for (int i = 0; i < 4; ++i)
			{
				if(FD_ISSET(ffd[i],&rfds))
				{
					read(ffd[i],buffer,maxbuf);

					if(i==1)
					{
						write(ffd[2],buffer,maxbuf);
					}else if(i==3)
					{
						write(ffd[0],buffer,maxbuf);
					}
				}
			}
		}else if(retval<0)
		{
			perror("select()");
			exit(1);
		}
	}





	return 0;
}