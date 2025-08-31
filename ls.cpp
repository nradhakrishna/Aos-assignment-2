#include "ls.h"
#include<iostream>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<unistd.h>
#include<iomanip>
#include<ctime>
using namespace std;

void print_all_details(string &path, struct stat& fileStat, string& name){
    cout<<(S_ISDIR(fileStat.st_mode)?'d':'-') ;
    //permissions
    cout<<((fileStat.st_mode & S_IRUSR)? 'r':'-');
    cout<<((fileStat.st_mode & S_IWUSR)? 'w' :'-');
    cout<<((fileStat.st_mode & S_IXUSR)? 'x': '-');
    cout<<((fileStat.st_mode & S_IRGRP)? 'r': '-');
    cout<<((fileStat.st_mode & S_IWGRP)? 'w' :'-');
    cout<<((fileStat.st_mode & S_IXGRP)? 'x':'-');
    cout<<((fileStat.st_mode & S_IROTH)? 'r': '-');
    cout<<((fileStat.st_mode & S_IWOTH)? 'w': '-');
    cout<<((fileStat.st_mode & S_IXOTH)? 'x' :'-');
    //links, owners, grps
    cout <<" "<< fileStat.st_nlink;
    cout <<" "<< getpwuid(fileStat.st_uid)->pw_name;
    cout <<' '<< getgrgid(fileStat.st_gid)->gr_name;
    cout << " " << setw(6) << fileStat.st_size;//pritns size of the file
    char timeBuf[80];
    strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", localtime(&fileStat.st_mtime));//gets last modified time
    cout << " " << timeBuf;
    cout << " " << name << endl;//prints name of the file
}


void list_file(string& path, bool a_flag, bool l_flag){
    struct stat fileStat;
    if(stat(path.c_str(), &fileStat)==-1){
        perror("ls");
        return;
    }
    string name=path;
    size_t pos=path.find_last_of('/');
    if(pos!=string::npos){
        name=path.substr(pos+1);
    }
    if(a_flag==false && name[0]=='.'){// visible files in the current directory
        return;
    }
    if(l_flag){//fire printalldetails function
        print_all_details(path, fileStat, name);
    }
    else{
        cout<< name<< endl;
    }
}
void list_files_in_dir(bool a_flag, string& path, bool l_flag){
    DIR* dir=opendir(path.c_str());
    if(!dir){
        perror("ls");
        return;
    }
    struct dirent* entry;
    while((entry=readdir(dir))!=NULL){
        string name=entry->d_name;
        if(a_flag==false && name[0]=='.')
        {
            continue;
        }
        string absolute_path=path+"/"+name;
        if(l_flag){
            struct stat fileStat;
            if(stat(absolute_path.c_str(), &fileStat)==-1){
                continue;
            }
            print_all_details(absolute_path, fileStat, name);
        }
        else{
            cout<< name<< endl;
        }

    }
    closedir(dir);
}

void fire_ls(vector<string>& arguments){
    bool a_flag=false;
    bool l_flag=false;
    vector<string> paths;
    for(int i=1;i<arguments.size();i++){
        string& arg=arguments[i];
        if(arg[0]=='-'){
            if(arg.find('a')!=string::npos){
                a_flag=true;
            }
            if(arg.find('l')!=string::npos){
                l_flag=true;
            }
        }
        else{
            paths.push_back(arg);
        }
    }
    if(paths.empty())
    {
        paths.push_back(".");
    }
    for(string& path: paths){
        struct stat st;
        if(stat(path.c_str(), &st)==-1){
            perror("ls");
            continue;
        }
        if(S_ISDIR(st.st_mode)){
            list_files_in_dir(a_flag, path, l_flag);
        }
        else{
            list_file(path, a_flag, l_flag);
        }
    }
}