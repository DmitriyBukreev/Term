#include "getch.h"

int getch(void)
{
	struct termios settings;
	cc_t min, time;
	int ch = 0;

	if (tcgetattr(0, &settings) < 0)
		return -1;
	// Exiting canonical mode
	settings.c_lflag &= ~ICANON;
	settings.c_lflag &= ~ECHO;
	min = settings.c_cc[VMIN];
	time = settings.c_cc[VTIME];
	settings.c_cc[VMIN] = 1;
	settings.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &settings) < 0)
		return -1;
	// Reading character
	READ_CHAR(ch);
	if (ch == START_CONTROL) { // Arrows codes
		READ_CHAR(ch);
		READ_CHAR(ch);
		switch (ch) {
		case 'D':
			ch = KEY_LEFT;
			break;
		case 'C':
			ch = KEY_RIGHT;
			break;
		default:
			ch = KEY_UNDEFINED;
		}
	}
	// Restoring settings
	settings.c_lflag |= ICANON;
	settings.c_lflag |= ECHO;
	settings.c_cc[VMIN] = min;
	settings.c_cc[VTIME] = time;
	if (tcsetattr(0, TCSANOW, &settings) < 0)
		return -1;
	return ch;
}