
#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

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
void checkJobs(vector<int> *processTable);
int printProcess(int process);
int checkZombieProcess(int process);
void printTime();
#endif