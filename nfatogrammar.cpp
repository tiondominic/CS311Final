#include "nfatogrammar.h"
#include <queue>
#include <iostream>
using namespace std;

void dfs(NFA &n,int s,vector<bool>&v,int &i){
    v[s]=true;
    n.states[s].symbol="S"+to_string(i++);
    for(auto &kv:n.states[s].transitions)
        for(int nx:kv.second)
            if(!v[nx]) dfs(n,nx,v,i);
}

void BFS(NFA &n){
    vector<bool> v(n.states.size(),false);
    queue<int> q;
    q.push(n.startState); v[n.startState]=true;

    while(!q.empty()){
        int u=q.front(); q.pop();
        for(auto &kv:n.states[u].transitions){
            for(int nx:kv.second){
                cout<<n.states[u].symbol<<"->"<<kv.first<<n.states[nx].symbol<<"\n";
                if(!v[nx]){ v[nx]=true; q.push(nx); }
            }
        }
        if(n.states[u].isFinal) cout<<n.states[u].symbol<<"->$\n";
    }
}

void convert(NFA &n){
    vector<bool> v(n.states.size(),false);
    int id=0;
    dfs(n,n.startState,v,id);
    BFS(n);
}
