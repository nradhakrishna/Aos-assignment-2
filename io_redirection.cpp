#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include "io_redirection.h"
using namespace std;


char* my_strdup_io(const string& s){
    char* dup = new char[s.size() + 1];
    strcpy(dup, s.c_str());
    return dup;
}

void handleIOredirection(vector<string> command){
    bool input=false, output=false, append=false;
    string input_file, output_file;
    vector<char *> pass_to_execvp;
    int n = command.size();
    int i = 0;

    while (i < n) {
        if (command[i] == "<") {
            input = true;
            if (i == n - 1) {
                cerr << "syntax error: no input file\n";
                _exit(1);
            }
            input_file = command[++i];
        }
        else if (command[i] == ">") {
            output = true;
            if (i == n - 1) {
                cerr << "syntax error: no output file\n";
                _exit(1);
            }
            output_file = command[++i];
        }
        else if (command[i] == ">>") {
            append = true;
            if (i == n - 1) {
                cerr << "syntax error: no output file\n";
                _exit(1);
            }
            output_file = command[++i];
        }
        else {
            pass_to_execvp.push_back(my_strdup_io(command[i]));
        }
        i++;
    }

    pass_to_execvp.push_back(nullptr);

    if (input) {
        int input_fd = open(input_file.c_str(), O_RDONLY);
        if (input_fd < 0) {
            perror("input file error");
            _exit(1);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output) {
        int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            perror("output file error");
            _exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    if (append) {
        int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (output_fd < 0) {
            perror("output file error");
            _exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    if (execvp(pass_to_execvp[0], pass_to_execvp.data()) == -1) {
        perror("execvp failed");
        _exit(1);
    }
}

void handlepipes(vector<vector<string>> temp_commands) {
    int n = temp_commands.size();
    if (n == 1) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            handleIOredirection(temp_commands[0]);
            _exit(0);
        } else {
            waitpid(pid, NULL, 0);
        }
        return;
    }
    int pipefd[2 * (n - 1)];
    for (int i = 0; i < n - 1; i++) {
        if (pipe(pipefd + i * 2) < 0) {
            perror("pipe failed");
            exit(1);
        }
    }
    vector<pid_t> pids;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            if (i > 0) 
            dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
            if (i < n - 1) 
            dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
            for (int j = 0; j < 2 * (n - 1); j++) 
            close(pipefd[j]);

            if (i==n-1) {
                bool input=false, output=false, append=false;
                string input_file, output_file;
                vector<char*> args;

                for (int k = 0; k < temp_commands[i].size(); k++) {
                    string token = temp_commands[i][k];
                    if (token == "<") { 
                        input = true; 
                        input_file = temp_commands[i][++k]; 
                    }
                    else if (token == ">") { 
                        output = true; 
                        output_file = temp_commands[i][++k]; 
                    }
                    else if (token == ">>") { 
                        append = true; 
                        output_file = temp_commands[i][++k]; 
                    }
                    else args.push_back(my_strdup_io(token));
                }
                args.push_back(nullptr);
                if (input) {
                    int fd=open(input_file.c_str(), O_RDONLY);
                    if (fd< 0) { 
                        perror("input file"); 
                        _exit(1); 
                    }
                    dup2(fd,STDIN_FILENO); 
                    close(fd);
                }
                if (output) {
                    int fd=open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd<0) { 
                        perror("output file"); 
                        _exit(1); 
                    }
                    dup2(fd,STDOUT_FILENO); 
                    close(fd);
                }
                if (append) {
                    int fd=open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd <0) { 
                        perror("append file"); 
                        _exit(1); 
                    }
                    dup2(fd, STDOUT_FILENO); close(fd);
                }
                execvp(args[0], args.data());
                perror("execvp failed");
                _exit(1);
            } else {
                vector<char*> args;
                for (auto &s : temp_commands[i]) 
                args.push_back(my_strdup_io(s));
                args.push_back(nullptr);

                execvp(args[0], args.data());
                perror("execvp failed");
                _exit(1);
            }
        } else {
            pids.push_back(pid);
        }
    }
    for (int j = 0; j < 2 * (n - 1); j++) 
    close(pipefd[j]);
    for (pid_t pid : pids) 
    waitpid(pid, NULL, 0);
}

void handlepipe_or_IO(vector<string> command){
    vector<vector<string>> ans;
    vector<string> current;
    int n=command.size();
    for(int i=0;i<n;i++){
        if(command[i]=="|"){
            ans.push_back(current);
            current.clear();
        }
        else{
            current.push_back(command[i]);
        }
    }
    if(current.empty()==false){
        ans.push_back(current);
    }
    handlepipes(ans);
}