#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <limits.h>
#include <wait.h>
#include <unistd.h>

#ifndef HANDLE_ERROR
#define HANDLE_ERROR(result, condition) \
do { \
	if ((result) == (condition)) { \
		perror("Error"); \
		exit(errno); \
	} \
} while (0)
#endif