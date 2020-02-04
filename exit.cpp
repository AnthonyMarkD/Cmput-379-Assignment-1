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

void exitShell379(vector<int> *processTable) {
	int status;
	pid_t process;
	for (int i = 0; i < processTable->size(); i++) {
		int state = kill((*processTable)[i], 0);
		if (state == 0) {
			// simply wait on all tasks
			do {
				process = waitpid((*processTable)[i], &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			
		} else if (state == ESRCH) {
			// does not exist

			processTable->erase(processTable->begin() + i);
		} else {
			// error
		}
	}
	exit(0);
}