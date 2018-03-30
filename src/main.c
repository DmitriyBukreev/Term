#include "input.h"
#define ARG_MAX 4096
#define ARG_NUM 128

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
	int arg_len;
	int stat_loc;
	char wait_f;

	input(buf);
	while (1) {
		printf("term: "); // Prompt
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

		// Simple "change dir" implementation
		if (!strcmp(args[0], "cd"))	{
			if (chdir(args[1]) == -1)
				perror("Error");
			continue;
		}

		// Setting flag to enable or disable wait()
		arg_len = strlen(args[0])-1;
		if (args[0][arg_len] == '&') {
			args[0][arg_len] = 0;
			wait_f = 0;
		} else
			wait_f = 1;

		pid = fork();
		HANDLE_ERROR(pid, -1);
		if (!pid) {
			// Child
			execvp(args[0], args);
			// Error
			perror("Error");
			exit(errno);
		}
		// Parent
		if (wait_f)
			wait(&stat_loc);
		else
			printf("[%i]\n", pid);
	}
}