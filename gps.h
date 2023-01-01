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
# define TZ_LOCAL 50000


int open_fd(void);
void set_port(int fd);
void menu(char *Pref);
void parse(char *out, char *Pref);
float kn_km(float knot);
float kn_mph(float knot);
float utc_loc(float utc);
char *heading(float deg);
float m_f(float m);
static int word_cnt(char const *s, char *c);
int char_cnt(char const *s, char *c);

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
/*
void parse(char *out, char *Pref)
{
	//rewrite parse func
    int len = strlen(out);
    int count = 0;
    int i = 0;
    char c[] = {',','*'};
    int sub = word_cnt(out, c);
	printf("%d\n", sub);

}
*/
void parse(char *out, char *Pref)
{ 
	//*** check to put substring in an array the proper way
	//*** need to parse needed information (which sentences) else all
	//add check first part of sentences to select parsing proper output
	char **to_parse;
	int i = 0;
	int j = 0;
	int k = 0;
	//char skip = ' ';
	char tmp_buf[16];
	char test[20];
	char test2[4];
	char c[] = {',','*'};

	to_parse = (char **)malloc(sizeof(char *) * word_cnt(out, c) + 1);
	if (!to_parse)
	{
		printf("Memory not allocated.\n");
		exit(0);
	}
	//while (out[i] != '\0' && out[i] != '\n' && i <= 100)
	while (out[i])
	{

		while (out[i] != ',' && out[i] != '*' && out[i] != '\n' && out[i] != '\0')
		{
			to_parse[j] = (char *)malloc(sizeof(char) * char_cnt(&out[i], c) + 1);
			if (!to_parse[j])
			{
				printf("oups!");
				exit(0);
			}
			//tmp_buf[j] = out[i];
			//tmp_buf[j + 1] = '\0';
			strncpy(to_parse[j++], &out[i], (char_cnt(out + i, c)));
			while (out[i] != ',' && out[i] != '*' && out[i] != '\n' && out[i] != '\0')
				i++;
			//j++;
		}
		//to_parse[j++] = (NULL);
		//if (out[i - 1] == ',' || out[i -1] == '*'){
				//tmp_buf[j] = '\0';
		//		to_parse[j++] = (NULL);
				//j++;
		//}
		//printf("tmp buf %s\t", tmp_buf);
		//printf("%s\n", *to_parse);

		k++;
		i++;
		//j = 0;
	}
	//here it's splited and ready to translate
	//may need to had char in empty space or check
	if (memcmp(&to_parse[0][0], Pref, 6) == 0)
	{
		strcpy(test, to_parse[1]);
		printf("test %s\n", test);
		
		strcpy(test2, to_parse[8]);
		
		printf("Title: %s \n", to_parse[0]);
		printf("Local Time: %.0lf\n ", utc_loc(atof(test)));
		printf("Heading %s %s\n", to_parse[8], heading(atof(test2)));
	}

	free(to_parse); // func free parsed[j]
}

float kn_km(float knot)
{
	return (knot * 1.852);
}

float kn_mph(float knot)
{
	return (knot * 1.150779);
}

float utc_loc(float utc)
{
	//***add system local timezone
	float local;
	local = (utc - TZ_LOCAL);
	return (local);
}

float m_f(float m)
{
	return (m * 3.28084);
}

char *heading(float deg)
{
	char *dir[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
	char *direction;

	if (deg == 0 || deg == 360)
		direction = dir[0];
	else if (deg > 0 && deg < 45)
		direction = dir[1];
	else if (deg == 45)
		direction = dir[2];
	else if (deg > 45 && deg < 90)
		direction = dir[3];
	else if (deg == 90)
		direction = dir[4];
	else if (deg > 90 && deg < 135)
		direction = dir[5];
	else if (deg == 135)
		direction = dir[6];
	else if (deg > 135 && deg < 180)
		direction = dir[7];
	else if (deg == 180)
		direction = dir[8];
	else if (deg > 180 && deg < 225)
		direction = dir[9];
	else if (deg == 225)
		direction = dir[10];
	else if (deg > 225 && deg < 270)
		direction = dir[11];
	else if (deg == 270)
		direction = dir[12];
	else if (deg > 270 && deg < 315)
		direction = dir[13];
	else if (deg == 315)
		direction = dir[14];
	else if (deg > 315 && deg < 360)
		direction = dir[15];
	//printf("%d\n", direction);
	//printf("%f\t%s\n", deg, dir[0]);
	return (direction);
}

static int word_cnt(char const *s, char *c)
{
	int word;
	int i;
	char temp[40];
	int j = 0;		//test

	i = 0;
	word = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c[0] && s[i] != c[1])
		{
			//word++;
			while (s[i] != c[0] && s[i] != c[1] && s[i] != '\0')
			{
				temp[j++] = s[i];
				i++;
			}
			if (s[i] == c[0] || s[i] == c[1] || s[i] == '\n')
            {
                word++;
                i++;
				//continue;
            }
			
		}
		else
        {
			i++;
            word++;
        }
		printf("w_cnt: \t%s\n", temp); // test
		j = 0;						//test
		bzero(temp, 40);			//test
	}
	printf("number of word: %d\n", word);
	return (word);
}

int char_cnt(char const *s, char *c)
{
	int cnt;

	cnt = 0;
	while (*s != c[0] && *s != '*' && *s != '\0')
	{
		cnt++;
		s++;
	}
	return (cnt);
}
#endif