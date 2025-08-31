#include "prompt.h"
#include<iostream>
#include<unistd.h>
#include<string>
using namespace std;


void printPrompt(string home_dir){
    char host_name[1024];
    gethostname(host_name, 1024);
    char cwd[1024];
    getcwd(cwd, 1024);
    const char *user_name=getenv("USER");
    cout<< string(user_name)<< "@"<< string(host_name)<<":";
    string curr_dir(cwd);
    if(curr_dir==home_dir){
        cout<<"~";
    }
    else{
        cout<< curr_dir;
    }
    cout<< ">";
}