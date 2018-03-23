#include "precompiled.h"
#define ARG_MAX 4096
#define ARG_NUM 128
#define EWRONGCOM 304

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
			return -1;
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
	pid_t pid;
	int stat_loc;

	while (1) {
		printf("term: ");
		HANDLE_ERROR(fgets(buf, ARG_MAX, stdin), NULL);
		if (buf[strlen(buf)-1] != '\n') {
			printf("Error: The input line is too long\n");
			getchar();
			continue;
		}
		if (parser(buf, args) == -1) {
			printf("Error: Too many arguments\n");
			continue;
		}

		pid = fork();
		HANDLE_ERROR(pid, -1);
		if (!pid) {
			// Child
			execvp(args[0], args);
			// Error
			printf("Сommand not found\n");
			exit(EWRONGCOM);
		}
		// Parent
		wait(&stat_loc);
	}
}