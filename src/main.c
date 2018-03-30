#include "getch.h"
#define ARG_MAX 4096
#define ARG_NUM 128

void input(char *buf)
{
	int ch;
	int iter = 0;
	int buf_total = 0;
	char ending[ARG_MAX];
	int ending_total = 0;

	while ((ch = getch()) > 0) {
		switch (ch) {
		case KEY_LEFT:
			if (iter > 0) {
				write(1, "\b", 1);
				iter--;
			}
			break;
		case KEY_RIGHT:
			if (iter < buf_total) {
				write(1, &buf[iter], 1);
				iter++;
			}
			break;
		default:
			if (buf_total < ARG_MAX - 1 && ch != '\n') {
				if (iter < buf_total && ending_total == 0) {
					ending_total = buf_total - iter;
					strncpy(ending, buf+iter, ending_total);
				}
				buf[iter] = ch;
				write(1, &ch, 1);
				if (iter < buf_total) {
					write(1, ending, ending_total);
					for (int i = ending_total; i > 0; i--)
						write(1, "\b", 1);
				}
				iter++;
				buf_total++;
			}
		}
		strncpy(buf+iter, ending, ending_total);
		ending_total = 0;
		if (ch == '\n') {
			buf[buf_total] = 0;
			break;
		}
	}
}

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
	char buf[ARG_MAX];
	char *args[ARG_NUM + 1];
	pid_t pid;
	int arg_len;
	int stat_loc;
	char wait_f;

	input(buf);
	printf("\n\n");
	printf("DEBUG_LINE: %s\n", buf);
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