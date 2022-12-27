#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>

struct termios term;

int main(void)
{
	int fd;
	char buf[100];
	int i;
	
	i = 0;	
	fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_SYNC);
	if (fd == -1)
	{
		printf("/dev/ttyUSB0 unavailable. %d\n", fd);
		exit (0);
	}

	tcgetattr(fd, &term);
	cfsetspeed(&term, B4800);
	tcsetattr(fd, TCSANOW, &term);

	while (fd)
	{
		i = 0;

		read(fd, &buf, 100);
		if (*buf == '$')
		{	
			printf("%s\n", &*buf);
			while (i < 100)
				buf[i++] = '\0';
		}
	}
	close(fd);
	return(0);
}