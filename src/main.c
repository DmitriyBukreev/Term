#include "precompiled.h"
#define ARG_MAX 4096
#define ARG_NUM 128
#define ETOOLONG 304
#define ETOOMANYARGS 305

// Splits the line into several strings with strtok
// and makes the array of them
// Returns number of strings + 1 (for NULL in the end)
// On error returns ETOOMANYARGS
int parser(char *src, char **dst)
{
	char *token = strtok(src, " \t\n");
	int iter = 0;

	while (token != NULL) {
		dst[iter] = token;
		if (iter == ARG_NUM)
			return -ETOOMANYARGS;
		iter++;
		token = strtok(NULL, " \t\n");
	}
	dst[iter] = NULL;
	return iter;
}

int main(int argn, char **argv)
{
	char buf[ARG_MAX] = "";
	char *args[ARG_NUM + 1];

	while (1) {
		HANDLE_ERROR(fgets(buf, ARG_MAX, stdin), NULL);
		if (buf[strlen(buf)-1] != '\n') {
			printf("Error: The input line is too long\n");
			return -ETOOLONG;
		}
		if (parser(buf, args) == ETOOMANYARGS) {
			printf("Error: Too many arguments\n");
			return -ETOOMANYARGS;
		}
		
	}
}