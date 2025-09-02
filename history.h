#ifndef HISTORY_H
#define HISTORY_H
#include <readline/history.h>
#include<string>
#include<deque>
using namespace std;

// int history_size=20;
const string history_file="history";
extern deque<string> d_h;


void maintain_history();
void print_history(int x, deque<string>& v_h);
void load_history();
void push_to_history(string c);
#endif