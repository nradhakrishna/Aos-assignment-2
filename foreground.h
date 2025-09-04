#ifndef FOREGROUND_H
#define FOREGROUND_H
#include<vector>
#include<signal.h>
#include<string>
using namespace std;
extern pid_t fg_pid;
void run_foreground(vector<string> command);
void int_handler(int signal);
void stop_handler(int signal);
#endif