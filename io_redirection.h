#ifndef IO_REDIRECTION
#define IO_REDIRECTION
#include<vector>
#include<string>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;
void handleIOredirection(vector<string> command);
#endif