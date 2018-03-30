#ifndef GETCH
#define GETCH
#include <termios.h>

#define READ_CHAR(ch) do { \
	if(read(0, &ch, 1) < 0) \
		return -1;			\
} while (0)

#define START_CONTROL 27
#define KEY_LEFT 129
#define KEY_RIGHT 130
#define KEY_UNDEFINED 304;

int getch(void);

#endif