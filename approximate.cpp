#include <queue>
#include <vector>
#include <tuple>
#include <fstream>
#include <filesystem>
#include <algorithm>
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

vector<MatchSpan> findAllApproxMatches(const NFA &nfa, const string &s, int kmax)
{
    vector<MatchSpan> matches;
    int L = s.size();

    for (int start = 0; start < L; ++start)
    {
        int N = nfa.states.size();
        vector<vector<vector<bool>>> vis(
            N, vector<vector<bool>>(L+1, vector<bool>(kmax+1,false)));

        queue<tuple<int,int,int>> q;

        pushClosure(nfa, nfa.startState, start, 0, q, vis);

        while (!q.empty()) {
            auto [state, pos, err] = q.front();
            q.pop();

            if (nfa.states[state].isFinal) {
                matches.push_back({start, pos});
            }

            if (pos < L) {
                char c = s[pos];
                if (nfa.states[state].transitions.count(c)) {
                    for (int nxt : nfa.states[state].transitions.at(c))
                        pushClosure(nfa, nxt, pos+1, err, q, vis);
                }
            }

            if (pos < L && err < kmax) {
                char c = s[pos];
                for (auto &kv : nfa.states[state].transitions) {
                    char a = kv.first;
                    if (a != '$' && a != c) { 
                        for (int nxt : kv.second)
                            pushClosure(nfa, nxt, pos+1, err+1, q, vis);
                    }
                }
            }

            if (pos < L && err < kmax) {
                int np = pos+1, ne = err+1;
                if (!vis[state][np][ne]) {
                    vis[state][np][ne] = true;
                    q.emplace(state, np, ne);
                }
            }

            if (err < kmax) {
                for (auto &kv : nfa.states[state].transitions) {
                    char a = kv.first;
                    if (a == '$') continue;
                    
                    for (int nxt : kv.second)
                        pushClosure(nfa, nxt, pos, err+1, q, vis);
                }
            }
        }
    }

    return matches;
}

string applyBrackets(const string &s, const vector<MatchSpan> &matches)
{
    map<int, pair<int, int>> bracketEvents; 

    for (const auto& match : matches) {
        bracketEvents[match.start].first++; 
        bracketEvents[match.end].second++;
    }

    string out = "";
    int L = s.size();
    for (int i = 0; i <= L; ++i) {
        
        if (bracketEvents.count(i)) {
            const auto& event = bracketEvents.at(i);
            
            for (int k = 0; k < event.second; ++k) {
                out += "]";
            }
            
            for (int k = 0; k < event.first; ++k) {
                out += "[";
            }
        }

        if (i < L) {
            out += s[i];
        }
    }

    return out;
}



void runApproxSearchAndWrite(const NFA &nfa, const string &input, int kmax)
{
    vector<MatchSpan> matches = findAllApproxMatches(nfa, input, kmax);

    if (matches.empty()) {
        writeOutput(input);
        return;
    }

    string marked = applyBrackets(input, matches);
    writeOutput(marked);
}