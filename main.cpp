#include<iostream>
#include "parser.h"
#include<unistd.h>
#include "prompt.h"
#include "ls.h"
#include "background.h"
#include "foreground.h"
#include <readline/readline.h>
#include "pinfo.h"
#include "history.h"
#include "search.h"

using namespace std;

string home_directory;
string prev_directory;
deque<string> d_h;
void change_directory(vector<string>& command){
    char cwd[1024];
    getcwd(cwd, 1024);
    string previous=prev_directory;
    prev_directory=string(cwd);
    if(command.size()==1){
        chdir(home_directory.c_str());
        return;
    }
    else if(command.size()==2){
        if(command[1]=="."){
            return;
        }
        else if(command[1]=="-"){
            chdir(previous.c_str());
            return;
        }
        else if(command[1]=="~"){
            chdir(home_directory.c_str());
            return;
        }
        if(chdir(command[1].c_str())!=0){
            perror("no such directory");
            return;
        }
    }
    else{
    perror("invalid arguments");
    }
}

void pwd(){
    string wd;
    char cwd[1024];
    getcwd(cwd, 1024);
    wd=string(cwd);
    cout<<wd<< endl;
}

void echo(vector<string>& command){
    for(unsigned int i=1;i<command.size();i++){
        cout<< command[i];
        if(i<command.size()-1)
        {
            cout<<" ";
        }
    }
    cout<< endl;
}

int main(){

    char curr_dir[1024];

    getcwd(curr_dir, 1024);

    home_directory=string(curr_dir);

    read_history(history_file.c_str());

    maintain_history();

    load_history();

    while(true){
        string prompt=returnPrompt(home_directory);
        char* ip=readline(prompt.c_str());
        string input(ip);
        free(ip);
        if(input.empty()==false){
            add_history(input.c_str());
            push_to_history(input);
            maintain_history();
        }
        vector<vector<string>> commands=parseCommands(input);
        for(auto &c: commands){
            if(c.size()==1 and c[0]=="exit"){
                write_history(history_file.c_str());
                return 0;
            }
            else if(c[0]=="cd"){
                change_directory(c);
            }
            else if(c[0]=="pwd"){
                pwd();
            }
            else if(c[0]=="echo"){
                echo(c);
            }
            else if(c[0]=="ls"){
                fire_ls(c);
            }
            else if(c[c.size()-1]=="&"){
                c.pop_back();
                // for(auto s:c){
                //     cout<< s<<endl;
                // }
                run_background(c);
                cout.flush();
                // run_background({"ls"});
            }
            else if(c[0]=="pinfo"){
                if(c.size()==1){
                    pinfo(0);
                }
                else if(c.size()==2){
                    pinfo(stoi(c[1]));
                }
                else{
                    perror("invalid args");
                }
            }
            else if(c[0]=="history"){
                if(c.size()==2){
                    print_history(stoi(c[1]), d_h);
                }
                else{
                print_history(20, d_h);
                }
            }
            else if(c[0]=="search"){
                handleSearch(c[1]);
            }
            else{
                run_foreground(c);
                cout.flush();
            }

        }
    }
    


    
    
}