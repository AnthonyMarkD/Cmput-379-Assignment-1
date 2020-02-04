#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <cstring>
#include <regex>
#include "operations.h"
#include "exit.h"
using namespace std;
const int LINE_LENGTH = 100; // Max # of charactersinan input line
const int MAX_ARGS = 7; // Max number of arguments to a command
const int MAX_LENGTH = 20; //Max # of characters in an argument
const int MAX_PT_ENTRIES = 32; //Max entries in the Process Table


struct rusage ru;
struct timeval user_time, system_time;

char input[100];
char* args[MAX_ARGS + 1];


int main() {
	vector<int> *processTable = new vector<int>();
	while (1) {
		// clear args
		memset(args, 0, sizeof(args));
		char * arg;
		cin.getline(input, sizeof(input));

		int num_tokens = 0;


		arg = strtok(input, " \t\n");



		while (arg != NULL)
		{
			args[num_tokens++] = arg;
			arg = strtok (NULL, " \t\n");
		}
		// set last arguement to null, execve needs it.
		args[num_tokens] = NULL;

		if (args[0] != NULL) {
			// parsing commands
			if (strcmp(args[0], "exit") == 0) {
				exitShell379(processTable);

			} else if (strcmp(args[0], "jobs") == 0) {
				checkJobs(processTable);

			}
			else if (strcmp(args[0], "kill") == 0) {
				kill(stoi(args[1]), SIGKILL);
			}
			else if (strcmp(args[0], "resume") == 0) {
				kill(stoi(args[1]), SIGCONT);
			}
			else if (strcmp(args[0], "sleep") == 0) {
				sleep(stoi(args[1]));
			}
			else if (strcmp(args[0], "suspend") == 0) {
				kill(stoi(args[1]), SIGSTOP);
			}
			else if (strcmp(args[0], "wait") == 0) {
				waitpid(stoi(args[1]), NULL, 0);
			} else {
				// We have entered an unsual command
				int status, i;
				pid_t newProcessPid = fork();
				pid_t waitNewProcess;
				if (newProcessPid == 0) {
					// child process
					execvp(args[0], args);
				} else if (newProcessPid > 0) {
					//parent process
					processTable->push_back(newProcessPid);
					//cout << "# of entries in newTable: " << processTable->size() << endl;

					if (strcmp(args[num_tokens - 1], "&") == 0) {
						// background baby

					} else {

						do {
							waitNewProcess = waitpid(newProcessPid, &status, WUNTRACED);
						} while (!WIFEXITED(status) && !WIFSIGNALED(status));
						processTable->pop_back();
					}
				} else if (newProcessPid < 0) {
					perror("An error has occured when attempting to fork.");
				}
			}
		}
	}
}

