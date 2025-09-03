#include "search.h"
bool search(path dir, string file_name){
    for(auto &entry: recursive_directory_iterator(dir)){
        if(entry.path().filename()==file_name){
            return true;
        }
    }
    return false;
}
void handleSearch(string file_name){
    path present=current_path();
    if(search(present, file_name)){
        cout<<"True"<< endl;
    }
    else{
        cout<< "False"<< endl;
    }
}