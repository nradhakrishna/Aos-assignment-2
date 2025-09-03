#include "history.h"
#include<iostream>
#include<fstream>

void maintain_history(){
    while(history_length>20){
        remove_history(0);
        // cout<<"maintaining history"<< endl;
    }
}

void print_history(int x, deque<string>& v_h){
    
    int limit=max(0, int(v_h.size())-x);
    limit=max(10, limit);
    int n=v_h.size();
    for(int i=n-1;i>=limit;i--){
        cout<< v_h[i]<< endl;
    }
    return;
}

void load_history(){
    ifstream in(history_file);
    string line;
    while(getline(in, line)){
        if(line.empty()==false){
            if((int)d_h.size()>=20){
                d_h.pop_front();
            }
            d_h.push_back(line);
        }
    }
}

void push_to_history(string c){
    d_h.push_back(c);
    if(d_h.size()>20){
        d_h.pop_front();
    }

}