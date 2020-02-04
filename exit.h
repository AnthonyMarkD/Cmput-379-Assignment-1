#ifndef _EXIT_H
#define _EXIT_H

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

using namespace std;
void exitShell379(vector<int> *processTable);

#endif