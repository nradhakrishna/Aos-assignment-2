#include "pinfo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include<sstream>
#include<vector>
using namespace std;
string my_trim(string s){
   size_t start=s.find_first_not_of(" \t");//finds the first index where it doesnot match with any of the given set of chars
   size_t end=s.find_last_not_of(" \t");// same as above but with the last index
   if(start==string::npos)
   {
    return "";
   } 
   return s.substr(start, end-start+1);
}
string readFile(const string& path){
    ifstream file(path);
    if(!file.is_open()){
        return "";
    }
    stringstream buffer;
    buffer<< file.rdbuf();
    return buffer.str();
}

string process_state(pid_t pid){
    string data=readFile("/proc/"+to_string(pid)+"/stat");
    if(data.empty()==true){
        return "None";
    }
    stringstream ss(data);
    int d,pg,session,tg,tty;
    string comm;
    char state;
    ss>> d>> comm>> state>> d>> pg>> session>> tty>>tg;
    string status(1, state);
    if(pg==tg){
        status+="+";
    }
    return status;
}
string virtual_memory(pid_t pid){
    string data=readFile("/proc/"+to_string(pid)+"/status");
    if(data.empty()==true){
        return "None";
    }
    string l;
    stringstream ss(data);
    while(getline(ss, l)){
        if(l.rfind("VmSize:", 0)==0){
            return my_trim(l.substr(7));
        }
    }
    return "None";
}
string executable_path(pid_t pid){
    string path="/proc/"+to_string(pid)+"/exe";
    char exe[PATH_MAX];
    ssize_t len=readlink(path.c_str(), exe, sizeof(exe)-1);
    if(len==-1)return "None";
    exe[len]='\0';
    return string(exe);
}
void pinfo(pid_t pid){
    if(pid==0){
        pid=getpid();
    }
    cout<<"Pid: "<< pid<< endl;
    cout<<"Process Status: "<< process_state(pid)<<endl;
    cout<<"Virtual Memory: "<< virtual_memory(pid)<<endl;
    cout<<"Executable Path: "<< executable_path(pid)<<endl;
    
}