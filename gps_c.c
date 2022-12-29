#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
/*If you run the program without any argument, every sentence will show on screen
if you want to print only one type of sentence run it as in exemple: "name of prog" \$GPGGA
this will only show $GPGGA sentence on screen

? print form with result displayed with an update frequency*/
struct termios term;

int openFd();
void setPort(int fd);

int main(int argc, char *argv[])
{

	char ref[100] = "";
	int input = 0;
	int fd;
	char buf[100];
	char gpgga[] = "$GPGGA";
	char gpgsa[] = "$GPGSA";
	char gpgsv[] = "$GPGSV";
	char gprmc[] = "$GPRMC";
	char test[] = "ALL";


	fd = openFd();
	setPort(fd);

	if (argc == 2)
	{
		strcpy(ref, argv[1]);
	}
	else
	{
		printf("type 1 for $GGPGA\ntype 2 for $GPGSA\ntype 3 for $GPGSV\ntype 4 for $GPRMC\n");
		printf("Select which data sentence you want to see:(0 to display everything.)\n");
		scanf(" %d", &input);

		switch (input){
			case (1):
				strcpy(ref, gpgga);
				break;
			case (2):
				strcpy(ref, gpgsa);
				break;
			case (3):
				strcpy(ref, gpgsv);
				break;
			case (4):
				strcpy(ref, gprmc);
				break;
			default:
				//strcpy(ref, test);
				break;
		}
	}

	while (fd)
	{
		read(fd, &buf, 100);
		if (*buf == '$')
		{
			char out[100];
			strcpy(out, buf);
			if (ref[0] != '$'){
				printf("%s\n", out);
			}	
			else{
				if (memcmp(buf, ref, 6) == 0)
				{
					printf("%s\n", out);
				}
			}
			bzero(buf, 100);
		}
	}	
	close(fd);

	return (0);
}

int openFd()
{
	int fd;
	fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_SYNC);

	if (fd == -1)
	{
		printf("/dev/ttyUSB0 unavailable. %d\n", fd);
		exit(0);
	}

	return (fd);
}

void setPort(int fd)
{
	tcgetattr(fd, &term);
	cfsetspeed(&term, B4800);
	tcsetattr(fd, TCSANOW, &term);
}