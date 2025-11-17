#include "regextonfa.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
using namespace std;

string expandCharClasses(const string &regex) {
    string out;
    size_t i = 0;
    while (i < regex.size()) {
        char c = regex[i];
        if (c == '\\') {
            if (i + 1 < regex.size()) {
                out += regex.substr(i, 2);
                i += 2;
            } else out += regex[i++];
        }
        else if (c == '[') {
            size_t j = i + 1;
            while (j < regex.size() && !(regex[j] == ']' && regex[j-1] != '\\')) j++;
            if (j == regex.size()) { out += '['; i++; continue; }

            string content = regex.substr(i+1, j-(i+1));
            vector<char> symbols;
            for (size_t k=0; k<content.size(); k++) {
                if (content[k]=='\\' && k+1<content.size()) {
                    symbols.push_back(content[++k]);
                } else if (k+2<content.size() && content[k+1]=='-') {
                    char a = content[k], b = content[k+2];
                    if (a<=b) for(char x=a;x<=b;x++) symbols.push_back(x);
                    else      for(char x=a;x>=b;x--) symbols.push_back(x);
                    k+=2;
                } else symbols.push_back(content[k]);
            }
            out.push_back('(');
            for (size_t k=0; k<symbols.size(); k++) {
                out.push_back(symbols[k]);
                if (k+1<symbols.size()) out.push_back('|');
            }
            out.push_back(')');
            i = j+1;
        }
        else out += regex[i++];
    }
    return out;
}

static vector<string> tokenizeAndInsertConcat(const string &r) {
    vector<string> t;
    for (size_t i=0;i<r.size();i++) {
        if (r[i]=='\\' && i+1<r.size()) {
            t.push_back(r.substr(i,2)); i++;
        } else {
            string s; s.push_back(r[i]); t.push_back(s);
        }
    }
    vector<string> out;
    auto lit=[&](string x){
        return !(x=="("||x==")"||x=="|"||x=="*");
    };
    for (size_t i=0;i<t.size();i++) {
        out.push_back(t[i]);
        if (i+1<t.size()) {
            bool A = lit(t[i]) || t[i]==")" || t[i]=="*";
            bool B = lit(t[i+1]) || t[i+1]=="(";
            if (A && B) out.push_back(".");
        }
    }
    return out;
}

static vector<string> toPostfix(const vector<string> &t) {
    vector<string> out, ops;
    auto prec=[&](string op){
        if(op=="*")return 3;
        if(op==".")return 2;
        if(op=="|")return 1;
        return 0;
    };
    for (auto tok : t) {
        if (tok=="(") ops.push_back(tok);
        else if (tok==")") {
            while (!ops.empty() && ops.back()!="(") {
                out.push_back(ops.back()); ops.pop_back();
            }
            if(!ops.empty()) ops.pop_back();
        }
        else if (tok=="*"||tok=="."||tok=="|") {
            while (!ops.empty() && prec(ops.back())>=prec(tok) && ops.back()!="(" && tok!="*") {
                out.push_back(ops.back()); ops.pop_back();
            }
            ops.push_back(tok);
        }
        else out.push_back(tok);
    }
    while(!ops.empty()){ out.push_back(ops.back()); ops.pop_back(); }
    return out;
}

struct Frag { int start, end; };
static int newState(vector<State> &st){ st.push_back(State()); return st.size()-1; }

NFA createNFA(const string &rRaw, int &curr) {
    string r=expandCharClasses(rRaw);
    if(r==""){
        NFA n; n.states.push_back(State()); n.states[0].isFinal=true;
        n.startState=n.endState=0; curr=1; return n;
    }

    auto tokens=tokenizeAndInsertConcat(r);
    auto post=toPostfix(tokens);

    vector<State> st;
    vector<Frag> S;

    for (auto &tok: post) {
        if (tok==".") {
            auto B=S.back(); S.pop_back();
            auto A=S.back(); S.pop_back();
            st[A.end].transitions['$'].push_back(B.start);
            S.push_back({A.start,B.end});
        }
        else if (tok=="|") {
            auto B=S.back(); S.pop_back();
            auto A=S.back(); S.pop_back();
            int s=newState(st), e=newState(st);
            st[s].transitions['$'].push_back(A.start);
            st[s].transitions['$'].push_back(B.start);
            st[A.end].transitions['$'].push_back(e);
            st[B.end].transitions['$'].push_back(e);
            S.push_back({s,e});
        }
        else if (tok=="*") {
            auto A=S.back(); S.pop_back();
            int s=newState(st), e=newState(st);
            st[s].transitions['$'].push_back(A.start);
            st[s].transitions['$'].push_back(e);
            st[A.end].transitions['$'].push_back(A.start);
            st[A.end].transitions['$'].push_back(e);
            S.push_back({s,e});
        }
        else {
            char c = (tok.size()==2 && tok[0]=='\\') ? tok[1] : tok[0];
            int s=newState(st), e=newState(st);
            st[s].transitions[c].push_back(e);
            S.push_back({s,e});
        }
    }

    NFA n;
    n.states=st;
    auto F=S.back();

    n.states[F.end].isFinal=true;
    n.startState=F.start;
    n.endState=F.end;

    for(size_t i=0;i<n.states.size();i++)
        for(auto &kv:n.states[i].transitions)
            if(kv.first!='$') n.alphabet.insert(kv.first);

    curr=n.states.size();
    return n;
}

set<int> epsilonClosure(const NFA& nfa, set<int> s) {
    set<int> c=s;
    queue<int> q;
    for(int x:s) q.push(x);
    while(!q.empty()){
        int u=q.front(); q.pop();
        if(nfa.states[u].transitions.count('$')){
            for(int v:nfa.states[u].transitions.at('$'))
                if(!c.count(v)){ c.insert(v); q.push(v); }
        }
    }
    return c;
}

bool validateString(const NFA& nfa,const string&s){
    if(nfa.states.empty())return false;
    set<int> cur=epsilonClosure(nfa,{nfa.startState});
    for(char c:s){
        set<int> nxt;
        for(int u:cur)
            if(nfa.states[u].transitions.count(c))
                for(int v:nfa.states[u].transitions.at(c)) nxt.insert(v);
        cur=epsilonClosure(nfa,nxt);
    }
    for(int u:cur) if(nfa.states[u].isFinal) return true;
    return false;
}
