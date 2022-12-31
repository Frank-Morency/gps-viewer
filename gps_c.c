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

#define PORT_SPEED B4800
#define PORT "/dev/ttyUSB0"

int open_fd(void);
void set_port(int fd);
void menu(char *Pref);
void parse(char *out, char *Pref);
float kn_km(float knot);
float kn_mph(float knot);
int utc_loc(int utc);
char heading(float deg);
float m_f(float m);
static int w_cnt(char const *s, char c);
int c_cnt(char const *s, char c);

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
					printf("%s", out);
			}
			bzero(buf, 100);
		}
	}
	close(fd);
	return (0);
}

int open_fd(void)
{
	int fd;

	fd = open(PORT, O_RDONLY | O_NOCTTY | O_SYNC);
	if (fd == -1)
	{
		printf("%s unavailable. %d\n", PORT, fd);
		exit(0);
	}
	return (fd);
}

void set_port(int fd)
{
	struct termios term;

	tcgetattr(fd, &term);
	cfsetspeed(&term, PORT_SPEED);
	tcsetattr(fd, TCSANOW, &term);
}

void menu(char *Pref)
{
	int input = 0;
	char gpgga[] = "$GPGGA";
	char gpgsa[] = "$GPGSA";
	char gpgsv[] = "$GPGSV";
	char gprmc[] = "$GPRMC";
	char test[] = "ALL";

	printf(" 1 for $GGPGA\n 2 for $GPGSA\n 3 for $GPGSV\n 4 for $GPRMC\n");
	printf(" Select sentence you want :(0 to display all.)\n");
	scanf(" %d", &input);
	switch (input)
	{
	case (1):
		strcpy(Pref, gpgga);
		break;
	case (2):
		strcpy(Pref, gpgsa);
		break;
	case (3):
		strcpy(Pref, gpgsv);
		break;
	case (4):
		strcpy(Pref, gprmc);
		break;
	default:
		strcpy(Pref, test); // not needed
		break;
	}
}

void parse(char *out, char *Pref)
{ // need to parse needed information (which sentences) else all
	int i = 0;
	int j = 0;
	int k = 0;
	char skip = ' ';
	char tmp_buf[16];
	char **parsed = (char **)malloc(sizeof(char *) * w_cnt(out, ','));
	if (!parsed)
	{
		printf("Memory not allocated.\n");
		exit(0);
	}
	while (out[i] != '\0' && out[i] != '\n' && i <= 100)
	{

		while (out[i] != ',' && out[i] != '\n' && out[i] != '\0')
		{
			parsed[j] = (char *)malloc(sizeof(char) * c_cnt(&out[i], ',') + 1);
			if (!parsed[j])
			{
				printf("oups!");
				exit(0);
			}
			tmp_buf[j] = out[i];
			tmp_buf[j + 1] = '\0';
			strncpy(parsed[j], &out[i], (c_cnt(out + i, ',')));
			
			i++;
			j++;
		}
		
		if (out[i - 1] == ','){
				tmp_buf[j] = '\0';
				*parsed[j] = '\0';
				j++;
		}
		//printf("i=%d, j=%d, k=%d\t",i, j, k);
		printf("tmp buf %s\t", tmp_buf);
		printf("%s\n", &**parsed);
		k++;
		i++;
		j = 0;
	}
	// put *out in an array splitted by coma?
	//?struct array 0= char 1= int 2= int 3= float 4= char etc...?
	//printf("%s-%s\n", parsed[0], *parsed);
	free(parsed);
}

float kn_km(float knot)
{
	return (knot * 1.852);
}

float kn_mph(float knot)
{
	return (knot * 1.150779);
}

int utc_loc(int utc)
{
	int local;
	local = -5;
	return (utc + local);
}

float m_f(float m)
{
	return (m * 3.28084);
}

char heading(float deg)
{
	char *dir[] = {"n", "ne", "e", "se", "s", "sw", "w", "nw"};
	char direction;

	if (deg == 0 || deg == 360)
		direction = *dir[0];
	else if (deg > 0 && deg < 90)
		direction = *dir[1];
	else if (deg == 90)
		direction = *dir[2];
	else if (deg > 90 && deg < 180)
		direction = *dir[3];
	else if (deg == 180)
		direction = *dir[4];
	else if (deg > 180 && deg < 270)
		direction = *dir[5];
	else if (deg == 270)
		direction = *dir[6];
	else if (deg > 270 && deg < 360)
		direction = *dir[7];
	return (direction);
}

static int w_cnt(char const *s, char c)
{
	int word;
	int i;

	i = 0;
	word = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			word++;
			while (s[i] != c && s[i] != '\0')
				i++;
			if (s[i] == c)
				continue;
		}
		else
			i++;
	}
	return (word);
}

int c_cnt(char const *s, char c)
{
	int cnt;

	cnt = 0;
	while (*s != c && *s != '\0')
	{
		cnt++;
		s++;
	}
	return (cnt);
}