#include <queue>
#include <vector>
#include <tuple>
#include "approximate.h"
#include "regextonfa.h"
using namespace std;

static void pushClosure(const NFA &nfa, int state, int pos, int err,
                        queue<tuple<int,int,int>> &q,
                        vector<vector<vector<bool>>> &vis)
{
    queue<int> w;
    vector<bool> seen(nfa.states.size(), false);
    w.push(state); seen[state]=true;

    while(!w.empty()){
        int s=w.front(); w.pop();
        if(!vis[s][pos][err]){
            vis[s][pos][err]=true;
            q.emplace(s,pos,err);
        }
        if(nfa.states[s].transitions.count('$')){
            for(int nxt:nfa.states[s].transitions.at('$'))
                if(!seen[nxt]){ seen[nxt]=true; w.push(nxt); }
        }
    }
}

bool validateApprox(const NFA &nfa,const string&s,int kmax){
    if(nfa.states.empty()) return false;
    int N=nfa.states.size(), L=s.size();

    vector<vector<vector<bool>>> vis(
        N, vector<vector<bool>>(L+1, vector<bool>(kmax+1,false)));

    queue<tuple<int,int,int>> q;
    pushClosure(nfa, nfa.startState, 0, 0, q, vis);

    while(!q.empty()){
        auto [state,pos,err]=q.front(); q.pop();

        if(pos==L && nfa.states[state].isFinal) return true;

        if(pos<L){
            char c=s[pos];
            if(nfa.states[state].transitions.count(c)){
                for(int nxt:nfa.states[state].transitions.at(c))
                    pushClosure(nfa,nxt,pos+1,err,q,vis);
            }
        }

        if(pos<L && err<kmax){
            char c=s[pos];
            for(auto &kv:nfa.states[state].transitions){
                char a=kv.first;
                if(a!='$' && a!=c){
                    for(int nxt:kv.second)
                        pushClosure(nfa,nxt,pos+1,err+1,q,vis);
                }
            }
        }

        if(pos<L && err<kmax){
            int np=pos+1, ne=err+1;
            if(!vis[state][np][ne]){
                vis[state][np][ne]=true;
                q.emplace(state,np,ne);
            }
        }

        if(err<kmax){
            for(auto &kv:nfa.states[state].transitions){
                char a=kv.first;
                if(a=='$') continue;
                for(int nxt:kv.second)
                    pushClosure(nfa,nxt,pos,err+1,q,vis);
            }
        }
    }

    for(int st=0;st<N;st++)
        if(nfa.states[st].isFinal)
            for(int e=0;e<=kmax;e++)
                if(vis[st][L][e]) return true;

    return false;
}
