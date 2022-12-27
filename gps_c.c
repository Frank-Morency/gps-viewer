#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
/*need to add command line argument to select whats printed on screen
? print form with result displayed with an update frequency*/
struct termios term;

int openFd();
void setPort(int fd);

int main(void)
{
	int fd;
	char buf[100];
	int i = 0;

	fd = openFd();	
	setPort(fd);

	while (fd)
	{
		i = 0;

		read(fd, &buf, 100);
		char ref[7] = "$GPGGA";

		if (*buf == '$')
		{	
			char out[100];
			strcpy(out, buf);
			if(memcmp(buf, ref, 6) == 0)
			{
				printf("%s\n",out);
			}
			//printf("*****%s\n",out);
			//printf("%s\n", &*buf);
			while (i < 100)
				buf[i++] = '\0';
		}
	}
	close(fd);

	return(0);
}

int openFd(){
	int fd;
	fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_SYNC);

	if (fd == -1)
	{
		printf("/dev/ttyUSB0 unavailable. %d\n", fd);
		exit (0);
	}

	return (fd);
}

void setPort(int fd){
	tcgetattr(fd, &term);
	cfsetspeed(&term, B4800);
	tcsetattr(fd, TCSANOW, &term);

}