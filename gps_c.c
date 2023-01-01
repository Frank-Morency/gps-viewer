#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include "gps.h"
/*if you want to pipe all sentence:
exemple: ./prog_name ALL | your_file.txt
For only one type of sentence run it as in:
exemple: ./prog_name \$GPGGA | your_file.txt
this will only take $GPGGA sentence on pipe.

If you dont put any argument you have to select at runtime*/
//better processing of argv for multiple sentence selection and to send selected data to file and/or screen
//add a test mode or file mode to read raw data from file


int main(int argc, char *argv[])
{
	char ref[100] = "x";
	char *Pref = ref;
	int fd;
	char buf[100];
	char out[100];

	fd = open_fd();
	set_port(fd);

	if (argc == 2)
		strcpy(ref, argv[1]);
	else
		menu(Pref);
	while (fd)
	{
		read(fd, &buf, 100);
		if (*buf == '$')
		{
			strcpy(out, buf);
			if (ref[0] != '$')
			{
				printf("%s", out);
//				parse(out, Pref);
//				exit(0);
			}
			else
			{
				if (memcmp(buf, ref, 6) == 0)
				{
					printf("%s", out);
//					parse(out, Pref);
				}	
			}
			bzero(buf, 100);
		}
	}
	close(fd);
	return (0);
}
