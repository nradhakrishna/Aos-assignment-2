#ifndef LS_H
#define LS_H
#include<string>
#include<vector>
using namespace std;
void fire_ls(vector<string>& args);
void list_files_in_dir(bool a_flag, string& path, bool l_flag);
void list_file(string& path, bool a_flag, bool l_flag);
#endif