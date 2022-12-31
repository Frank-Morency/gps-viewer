#ifndef GPS_H
# define GPS_H
# include<stdlib.h>
# include<stdio.h>
# include<fcntl.h>
# include<unistd.h>
# include<termios.h>
# include<string.h>



# define PORT_SPEED B4800
# define PORT "/dev/ttyUSB0"
# define TZ_LOCAL -5


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
	//add a selection to save to file
	//add a selection for parsed or raw
	//add a selection for a kinda tui output
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
{
	//rewrite parse func

}
/*
void parse(char *out, char *Pref)
{ 
	//*** check to put substring in an array the proper way
	//*** need to parse needed information (which sentences) else all
	//add check first part of sentences to select parsing proper output
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
		printf("tmp buf %s\t", tmp_buf);
		printf("%s\n", &**parsed);
		k++;
		i++;
		j = 0;
	}

	free(parsed);
}
*/
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
	//***add system local timezone
	int local;
	local = TZ_LOCAL;
	return (utc + local);
}

float m_f(float m)
{
	return (m * 3.28084);
}

char heading(float deg)
{
	char *dir[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
	char direction;

	if (deg == 0 || deg == 360)
		direction = *dir[0];
	else if (deg > 0 && deg < 45)
		direction = *dir[1];
	else if (deg == 45)
		direction = *dir[2];
	else if (deg > 45 && deg < 90)
		direction = *dir[3];
	else if (deg == 90)
		direction = *dir[4];
	else if (deg > 90 && deg < 135)
		direction = *dir[5];
	else if (deg == 135)
		direction = *dir[6];
	else if (deg > 135 && deg < 180)
		direction = *dir[7];
	else if (deg == 180)
		direction = *dir[8];
	else if (deg > 180 && deg < 225)
		direction = *dir[9];
	else if (deg == 225)
		direction = *dir[10];
	else if (deg > 225 && deg < 270)
		direction = *dir[11];
	else if (deg == 270)
		direction = *dir[12];
	else if (deg > 270 && deg < 315)
		direction = *dir[13];
	else if (deg == 315)
		direction = *dir[14];
	else if (deg > 315 && deg < 360)
		direction = *dir[15];
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
#endif