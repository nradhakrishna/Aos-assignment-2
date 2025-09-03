#include "io_redirection.h"
void makeTrue(bool &x){
    x=true;
}

char* my_strdup_io(const string& s){
    int len = s.size() + 1;
    char* dup = new char[len];
    for(int i = 0; i < s.size(); i++) dup[i] = s[i];
    dup[len-1] = '\0';
    return dup;
}

void handleIOredirection(vector<string> command){
    bool input=false, output=false, append=false;
    string input_file, output_file;
    vector<char *> pass_to_execvp;
    int n=command.size();
    int i=0;
    while(i<n){
        if(command[i]=="<"){
            makeTrue(input);
            if(i==n-1){
                perror("invalid args");
                return;
            }
            i++;
            input_file=command[i];
            i++;
        }
        else if(command[i]==">"){
            makeTrue(output);
            if(i==n-1){
                perror("invalid args");
                return;
            }
            i++;
            output_file=command[i];
            i++;
        }
        else if(command[i]==">>"){
            makeTrue(append);
            if(i==n-1){
                perror("invalid args");
                return;
            }
            i++;
            output_file=command[i];
            i++;
        }
        else{
            pass_to_execvp.push_back(my_strdup_io(command[i].c_str()));
            i++;
        }
    }
    pass_to_execvp.push_back(nullptr);
    pid_t pid=fork();
    if(pid==0){
        if(input){
            int input_fd;
            input_fd=open(input_file.c_str(), O_RDONLY);
            if(input_fd<0){
                perror("input file error");
                exit(1);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if(output){
            int output_fd;
            output_fd=open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(output_fd<0){
                perror("output file error");
                exit(1);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        if(append){
            int output_fd;
            output_fd=open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(output_fd<0){
                perror("output file error");
                exit(1);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        if(execvp(pass_to_execvp[0], pass_to_execvp.data())==-1){
            perror("execvp failed");
            exit(1);
        }
        

    }
    else if(pid>0){
        waitpid(pid, NULL, 0);
    }
    else{
        perror("fork failed");
        return;
    }

}
