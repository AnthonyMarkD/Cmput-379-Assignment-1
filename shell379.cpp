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
using namespace std;
const int LINE_LENGTH = 100; // Max # of charactersinan input line
const int MAX_ARGS = 7; // Max number of arguments to a command
const int MAX_LENGTH = 20; //Max # of characters in an argument
const int MAX_PT_ENTRIES = 32; //Max entries in the Process Table

struct Process {
	int pid;
	string command;

} ;
struct rusage ru;
struct timeval user_time, system_time;
vector<Process> processTable;
char input[100];
char* args[MAX_ARGS + 1];
string userInput, first_arg, second_arg, third_arg;

void checkJobs();
void printProcess(Process process);
int main() {

	while (1) {
		cin.getline(input, sizeof(input));
		
		int num_tokens = 0;
 	
		char * arg = strtok(input, " \t\n");
		while (arg != NULL)
		{
			args[num_tokens++] = arg;
			arg = strtok (NULL, " \t\n");
		}
		args[num_tokens] = NULL;
		if (strcmp(args[0], "exit") == 0) {

		} else if (strcmp(args[0], "jobs") == 0) {

			checkJobs();
		}
		else if (strcmp(args[0], "kill") == 0) {
			kill(stoi(args[1]), 0);
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

		} else if (strcmp(args[0], "") == 0) {
			// do nothing
		} else {
			int status, i;
			pid_t newProcessPid = fork();
			if (newProcessPid == 0) {
				/* child process */
				execvp(args[0], args);
				cout << endl;

			} else {
				/* parent process */
				
				//wait(&status);

				Process process = {newProcessPid, userInput};
				processTable.push_back(process);
			}
		}
	}

}
void checkJobs() {
	cout << "Running processes:" << endl;
	cout << "#    PID S SEC COMMAND" << endl;
	int activeJobs = 0;
	for (int i = 0; i < processTable.size(); i++) {
		int state = kill(processTable[i].pid, 0);
		if (state == 0) {
			activeJobs++;
			printProcess(processTable[i]);
			// print process
			// exists
		} else if (state == ESRCH) {
			// does not exist
			processTable.erase(processTable.begin() + i);
		} else {
			// error
		}
	}
	printf("Processes =     %d active\n", activeJobs);

	cout << "Completed processes:" << endl;
	printf("User time =     %d seconds\n", activeJobs);
	printf("Sys  time =     %d seconds\n", activeJobs);


}
void printProcess(Process process) {
	FILE * pfile;
	int c;
	char buff[200];
	string pid = to_string(process.pid);

	//sprintf(buff, "ps -p %d -o sched=#,pid,s,cputime=SEC,command", process.pid);
	string args =  "ps -p " + pid + " -o sched=#,pid,s,cputime=SEC,command | awk -F'[: ]+' '/:/ {t=d$4+60*(d$5+60*d$6); printf (\"%s: %s %s   %s %s %s %s\",d$2,d$3,d$4,t,d$8,d$9,d$10)}'";
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
			if ( putchar( c ) == EOF ) {
				perror( "Putchar failure\n" );
				break;
			}
		}

		if ( pclose( pfile ) ) {
			perror( "Pipe close failure\n" );
		}
	}
	cout << endl;
}