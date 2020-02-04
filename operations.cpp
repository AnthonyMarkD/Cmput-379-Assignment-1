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
using namespace std;
regex pat{ "(\\d+:   \\d+ Z )" };
void checkJobs(vector<int> *processTable) {
	cout << "Running processes:" << endl;

	int activeJobs = 0;

	for (int i = 0; i < processTable->size(); i++) {
		int state = kill((*processTable)[i], 0);
		if (state == 0) {


			activeJobs++;

			// print process
			// exists
		}

	} if (activeJobs > 0) {
		cout << "#    PID S SEC COMMAND" << endl;
		for (int i = 0; i < processTable->size(); i++) {
			int state = kill((*processTable)[i], 0);
			if (state == 0) {

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
	}

	printf("Processes =     %d active\n", activeJobs);

	cout << "Completed processes:" << endl;
	printTime();


}

void printProcess(int process) {
	FILE * pfile;
	int c;

	string pid = to_string(process);
	string output;
	//sprintf(buff, "ps -p %d -o sched=#,pid,s,cputime=SEC,command", process.pid);
	string args =  "ps -p " + pid + " -o sched=#,pid,s,cputime=SEC,command | awk -F'[: ]+' '/:/ {t=d$4+60*(d$5+60*d$6); printf (\"%s:   %s %s   %s %s\",d$2,d$3,d$4,t,d$8)}'";
	int n = args.length();

	// declaring character array
	char char_array[n + 1];

	// copying the contents of the
	// string to char array
	strcpy(char_array, args.c_str());
	// Open the pipe. The pipe's output (i.e., the results of the "sort" command
	// will be available for reading by this program (accessible as FILE "pfile")
	if ( ( pfile = popen(char_array, "r" ) ) == NULL ) {
		perror( "Pipe open failure\n" );
	} else {

		// Read the the pipe until end-of-file is reached.
		while ( ( c = fgetc( pfile ) ) != EOF ) {
			output += (char)c;
		}
		// no more zombies that you...
		bool match = regex_search(output, pat);
		if (match) {
			waitpid(process, NULL, 0);
		} else {
			cout << output << endl;
		}


		if ( pclose( pfile ) ) {
			perror( "Pipe close failure\n" );
		}
	}


	cout << endl;
}
void printTime() {
	int who = RUSAGE_CHILDREN;
	struct rusage usageStat;
	int result = getrusage(who, &usageStat);
	printf("User time =     %ld seconds\n", usageStat.ru_utime.tv_sec);
	printf("Sys  time =     %ld seconds\n", usageStat.ru_stime.tv_sec);
}