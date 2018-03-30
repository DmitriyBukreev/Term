#ifndef GETCH
#define GETCH
#include <termios.h>

#define READ_CHAR(ch) do { \
	if(read(0, &ch, 1) < 0) \
		return -1;			\
} while (0)

#define START_CONTROL 27
#define KEY_BACKSPACE 127
#define KEY_LEFT 128
#define KEY_RIGHT 129
#define KEY_UNDEFINED 304;

int getch(void);

#endif