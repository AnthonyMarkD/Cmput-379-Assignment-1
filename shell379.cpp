#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

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
char arg1[] = "/bin/sh";
char arg2[] = "-c";
string userInput, first_arg, second_arg, third_arg;

void checkJobs();
void printProcess(Process process);
int main() {

	while (1) {
		cin.getline(input, sizeof(input));
		userInput = input;
		first_arg = userInput.substr(0, userInput.find(' '));
		second_arg = userInput.substr(userInput.find(' ') + 1, userInput.find(' '));


		if (first_arg == "exit") {

		} else if (first_arg == "jobs") {

			checkJobs();
		}
		else if (first_arg == "kill") {
			kill(stoi(second_arg), 0);
		}
		else if (first_arg == "resume") {

		}
		else if (first_arg == "sleep") {
			sleep(stoi(second_arg));
		}
		else if (first_arg == "suspend") {

		}
		else if (first_arg == "wait") {

		}else if (first_arg == "") {
			// do nothing
		} else {
			int status, i;
			pid_t newProcessPid = fork();
			if (newProcessPid == 0) {
				/* child process */

				char *args[] = { arg1, arg2, input, NULL};
				execvp("/bin/sh", args);
				cout << endl;
				
			} else {
				/* parent process */

				//wait(&status);


				printf("Parent done!\n");
				Process process = {newProcessPid, userInput};
				processTable.push_back(process);
			}
		}
	}

}
void checkJobs() {
	cout << "Running processes:" << endl;
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
	char buff[100];
	sprintf(buff,"ps -p %d -o sched=#,pid,s,cputime=SEC,command", process.pid);

	// Open the pipe. The pipe's output (i.e., the results of the "sort" command
	// will be available for reading by this program (accessible as FILE "pfile")
	if ( ( pfile = popen(buff, "r" ) ) == NULL ) {
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
}