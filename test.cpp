#include<iostream>
#include<string>

#include<unistd.h>
using namespace std;

string home_dir;

string getCommand(){
    string input;
    getline(cin,input);
    return input;
}



void printPrompt(){
    char host_name[1024];
    char cwd[1024];
    gethostname(host_name, 1024);
    getcwd(cwd, 1024);
    const char* user_name=getenv("USER");
    string curr_dir(cwd);
    cout<< string(user_name)<<"@"<< string(host_name)<<":";
    if(curr_dir==home_dir){
        cout<<"~"<<">";
    }
    else{
        cout<< curr_dir<<">";
    }
}

void change_directory(const string& command){
    if(command.size()==2){
        chdir(home_dir.c_str());
        return;
    }
    string path=command.substr(3);
    if(path.empty()){
        chdir(home_dir.c_str());
        return;
    }
    if(chdir(path.c_str())!=0){
        perror("cd failed");
    }


}

void handlepwd(){
    char cwd[1024];
    getcwd(cwd, 1024);
    string s_cwd(cwd);
    if(s_cwd.empty()){
        perror("pwd failed");
    }
    else{
        cout<< s_cwd<< endl;
    }

}

int main(){

    string command="";
    char cwd[1024];
    home_dir=string(getcwd(cwd, 1024));
    while(command!="exit"){
    printPrompt();
    command=getCommand();

    if(command.rfind("cd",0)==0){
        change_directory(command);
    }
    else if(command=="pwd"){
        handlepwd();
    }
    }
    return 0;
}