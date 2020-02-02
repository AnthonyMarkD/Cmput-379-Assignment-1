#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define LINE_LENGTH		100
#define MAX_ARGS		  7
#define MAX_LENGTH		 20
#define MAX_PT_ENTRIES	 32

char* getInput() {

	printf("ENTRY 3.1");

	char* input;

	printf("ENTRY 3.2");

	while(fgets(input, LINE_LENGTH, stdin) == NULL) { }

	printf("ENTRY 3.3");

	return input;
}

int main() {

	printf("ENTRY 1");

	pid_t* processTable[MAX_PT_ENTRIES];
	int* pt_entries; *pt_entries = 0;

	printf("ENTRY 2");

	char line[LINE_LENGTH];
	char* tokens[MAX_ARGS + 1];

	printf("ENTRY 3");

	while (true) {

		fflush(stdin);
		fflush(stdout);

		strcpy(line, getInput());

		printf("ENTRY 4");

		// Split the line into tokens

		int num_tokens = 0;
		char* token = strtok(line, " \t\n");
		while (token && strlen(token) <= MAX_LENGTH) {
			tokens[num_tokens++] = token;
			token = strtok(NULL, " \t\n");
		}
		if (strlen(token) > MAX_LENGTH) {
			perror("Token length max exceeded.");
			continue;
		}
		if (num_tokens > MAX_ARGS) {
			perror("Maximum number of tokens exceeded.");
			continue;
		}
		tokens[num_tokens] = NULL;

		printf("ENTRY 5");

		// Fork the process and add to the process table

		pid_t pid = fork();
		int status;
		pid_t wpid;

		if (pid > 0) {
			*(processTable[(*pt_entries)++]) = pid;
			if (strcmp(tokens[num_tokens - 1], "&") != 0) {
				// If the second last token is not a '&' we wait for the process
				do {
					wpid = waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				// Once the process is finished, we don't need the process in the table anymore
				processTable[--(*pt_entries)] = NULL;
			}
		} else if (pid < 0) {
			perror("Error forking.");
			continue;
		} else {

			// Check for valid commands

			if (strcmp(tokens[0], "jobs\n") == 0) {
				
				printf("\nRunning processes:\n");

				// Print process info
				if (*pt_entries > 0) {
					printf("#    PID S SEC COMMAND");

					for (int p = 0; p < *pt_entries; p++) {
						int ret = kill(*processTable[p], 0);
						// if (ret == ESRCH) {
						// 	// The process does not exist
						// 	// Remove the process from the process table
						// 	processTable[p] = processTable[(*pt_entries) - 1];
						// 	processTable[(*pt_entries) - 1] = NULL;
						// 	(*pt_entries)--;
						// 	p--;
						// } else
						if (ret == 0) {
							// The process returned no error, it exists
						} else {
							// The process does not exist
							// Remove the process from the process table
							processTable[p] = processTable[(*pt_entries) - 1];
							processTable[(*pt_entries) - 1] = NULL;
							(*pt_entries)--;
							p--;

							// // The process returned an error when signaling
							// perror("Error signalling process.");
						}
					}

					FILE* pfile;
					int c;

					char command[] = "ps -a -o sched=#,pid,s,cputime=SEC,command";

					// char command[8] = "ps ";
					// char pidAsStr[5];
					// itoa((int) *processTable[p], pidAsStr, 10);
					// strcat(command, pidAsStr);

					if ((pfile = popen(command, "r")) == NULL) {
						perror("Pipe open failure\n");
					} else {
						while ((c = fgetc(pfile)) != EOF) {
							if (putchar(c) == EOF) {
								perror("Putchar failure\n");
								break;
							}
						}
						if (pclose(pfile)) {
							perror("Pipe close failure\n");
						}						
					}

				}

				printf("Processes = %6d active\n", *pt_entries);
				printf("Completed processes:\n");
				// printf("User time = %6d seconds\n", (int) *userTime);
				// printf("Sys  time = %6d seconds\n\n", (int) *sysTIme);

			} else {
				// printf("Not a valid command.\n");
				execvp(tokens[0], tokens);
			}

		}

	}

	return 0;
}
