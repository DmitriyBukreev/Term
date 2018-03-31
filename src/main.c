#define ARG_MAX 4096
#define INFO_MAX 512
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

pid_t pid;
void kill_child(int sig)
{
	if (pid != 0)
		HANDLE_ERROR(kill(pid, sig), -1);
}

int main(int argn, char **argv)
{
	char buf[ARG_MAX] = "";  // Input buffer
	char *user;				// Info for prompt
	char host[INFO_MAX];
	char dir[INFO_MAX];
	char *args[ARG_NUM + 1]; // Parsed tokens
	char wait_f;			 // Flag setted by '&'
	int arg_len;			 // Length of first argument
	int stat_loc;
	int parsed_num = 0;
	struct passwd *uidinfo;  // Info about current user

	// Initialization
	uidinfo = getpwuid(getuid());
	if (uidinfo == NULL) {
		perror("Error");
		user = "USER_NOT_DEFINED";
	} else
		user = uidinfo->pw_name;

	if (gethostname(host, INFO_MAX) == -1) {
		perror("Error");
		strcpy(host, "HOST_NOT_DEFINED");
	}

	(void) signal(SIGINT, SIG_IGN);
	while (1) {
		if (getcwd(dir, INFO_MAX) == NULL) {
			perror("Error");
			strcpy(dir, "DIR_NOT_DEFINED");
		}
		printf("%s@%s: %s$ ", user, host, dir); // Prompt
		HANDLE_ERROR(fgets(buf, ARG_MAX, stdin), NULL);
		if (buf[strlen(buf)-1] != '\n') {
			printf("Error: The input line is too long\n");
			getchar();
			continue;
		}
		parsed_num = parser(buf, args);
		if (parsed_num == -1) {
			printf("Error: Too many arguments\n");
			continue;
		} else if (parsed_num == 0)
			continue;

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
		if (wait_f) {
			wait(&stat_loc);
			(void) signal(SIGINT, SIG_IGN);
		} else {
			printf("[%i]\n", pid);
			(void) signal(SIGINT, kill_child);
		}

	}
}