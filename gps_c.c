#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
/*if you want to pipe all sentence:
exemple: ./prog_name ALL | your_file.txt
only one type of sentence run it as in:
exemple: ./prog_name \$GPGGA | your_file.txt
this will only take $GPGGA sentence on pipe.

If you dont put any argument you have to select at runtime*/

int		open_fd(void);
void	set_port(int fd);
void	menu(char *Pref);

int	main(int argc, char *argv[])
{
	char	ref[100] = " ";
	char	*Pref = ref;
	int		fd;
	char	buf[100];
	char	out[100];
	

	fd = open_fd();
	set_port(fd);

	if (argc == 2)
	{
		strcpy(ref, argv[1]);
	}
	else
	{
		menu(Pref);
	}
	while (fd)
	{
		read(fd, &buf, 100);
		if (*buf == '$')
		{	
			strcpy(out, buf);
			if (ref[0] != '$')
			{
				printf("%s", out);
			}	
			else
			{
				if (memcmp(buf, ref, 6) == 0)
				{
					printf("%s", out);
				}
			}
			bzero(buf, 100);
		}
	}	
	close(fd);
	return (0);
}

int	open_fd(void)
{
	int	fd;

	fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY | O_SYNC);
	if (fd == -1)
	{
		printf("/dev/ttyUSB0 unavailable. %d\n", fd);
		exit(0);
	}
	return (fd);
}

void	set_port(int fd)
{
	struct termios	term;

	tcgetattr(fd, &term);
	cfsetspeed(&term, B4800);
	tcsetattr(fd, TCSANOW, &term);
}
void menu(char *Pref){
	int		input = 0;
	char	gpgga[] = "$GPGGA";
	char	gpgsa[] = "$GPGSA";
	char	gpgsv[] = "$GPGSV";
	char	gprmc[] = "$GPRMC";
	char	test[] = "ALL";

	printf("type 1 for $GGPGA\ntype 2 for $GPGSA\ntype 3 for $GPGSV\ntype 4 for $GPRMC\n");
		printf("Select which data sentence you want to see:(0 to display everything.)\n");
		scanf(" %d", &input);
		switch (input)
		{
			case (1) :
			{
				strcpy(&*Pref, gpgga);
				break ;
			}
			case (2) :
			{
				strcpy(&*Pref, gpgsa);
				break ;
			}
			case (3) :
			{
				strcpy(&*Pref, gpgsv);
				break ;
			}
			case (4) :
			{
				strcpy(&*Pref, gprmc);
				break ;
			}
			default :
				break ;
		}
}