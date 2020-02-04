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
void checkJobs(vector<int> *processTable) {
	cout << "Running processes:" << endl;
	cout << "#    PID S SEC COMMAND" << endl;
	int activeJobs = 0;
	for (int i = 0; i < processTable->size(); i++) {
		int state = kill((*processTable)[i], 0);
		if (state == 0) {


			activeJobs++;
			printProcess((*processTable)[i]);
			// print process
			// exists
		} else if (state == ESRCH) {
			// does not exist
			processTable->erase(processTable->begin() + i);
		} else {
			// error
		}
	}
	printf("Processes =     %d active\n", activeJobs);

	cout << "Completed processes:" << endl;
	printf("User time =     %d seconds\n", activeJobs);
	printf("Sys  time =     %d seconds\n", activeJobs);


}