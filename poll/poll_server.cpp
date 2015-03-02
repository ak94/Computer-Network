#include "poll.h"
#include "bits/stdc++.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

using namespace std;
 
#define fifo_a "fifo_a"
#define fifo_b "fifo_b"
#define fifo_c "fifo_c"
#define fifo_d "fifo_d"

#define maxbuf 1024

struct pollfd pfd[2];
int timeout_msecs = 500;
int ret;

int main(int argc, char const *argv[])
{
	
	
/**
 * establish fifo
 */
 	int ffd[4];

 	mkfifo(fifo_a,0666);
 	mkfifo(fifo_b,0666);
 	mkfifo(fifo_c,0666);
 	mkfifo(fifo_d,0666);

 	ffd[0]=open(fifo_a,O_WRONLY);
 	ffd[1]=open(fifo_b,O_RDONLY);
 	ffd[2]=open(fifo_c,O_WRONLY);
 	ffd[3]=open(fifo_d,O_RDONLY);

	pfd[0].fd = open(fifo_b,O_RDONLY);
	pfd[0].events = POLLIN; 

	pfd[1].fd = open(fifo_d,O_RDONLY);
	pfd[1].events = POLLIN;
	while(1)
	{
		ret = poll(pfd,2,timeout_msecs);
		
	if(ret>0)
	{
		for (int i = 0; i < 2; ++i)
		{
			if(pfd[i].revents && POLLIN)
			{
				pfd[i].revents=0;

				char buffer[maxbuf];
				read(pfd[i].fd,buffer,maxbuf);

				if(i==0)
				{
					write(ffd[2],buffer,maxbuf);
				}else if(i==1)
				{
					write(ffd[0],buffer,maxbuf);
				}
			}

			if (pfd[i].revents & POLLHUP) 
			{
				perror("POLLHUP error");
			}
		}
	}
	
	}
	return 0;
}