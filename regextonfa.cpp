#include "regextonfa.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <bits/stdc++.h>
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


vector<string> parseSegments(string a)
{
    int count = 0;
    string temp = "";
    vector<string> seg;
    stack<string> operators;
    string regExp = expandCharClasses(a);

    for (size_t i = 0; i < regExp.length(); i++)
    {
        char c = regExp[i];

        if (c == '(')
        {
            count++;
            if (count == 1)
                continue;
        }
        else if (c == ')')
        {
            count--;
            if (count == 0)
            {
                seg.push_back(temp);
                temp.clear();

                if (i < regExp.length() - 1)
                {
                    char next = regExp[i + 1];
                    if (next != '*' && next != '|' && next != ')')
                        seg.push_back(".");
                }
                continue;
            }
        }

        temp += c;
        if (count == 0)
        {
            seg.push_back(temp);
            temp.clear();

            if (i < regExp.length() - 1)
            {
                char next = regExp[i + 1];
                if (regExp[i] != '|' && next != '*' && next != '|' && next != ')')
                    seg.push_back(".");
            }
        }
    }
    return seg;
}

NFA createNFA(const string &regex, int &curr)
{
    NFA temp;
    if (regex == "#")
    {
        temp.states.push_back(State());
        return temp;
    }
    else if (regex == "")
    {
        State s;
        s.isFinal = true;
        temp.states.push_back(s);
        temp.alphabet.insert('$');
        return temp;
    }
    else if (regex.length() == 1)
    {
        temp.states.push_back(State());
        temp.states.push_back(State());

        temp.states[0].transitions[regex[0]].push_back(curr + 1);
        temp.states[1].isFinal = true;

        temp.startState = curr;
        temp.endState = curr + 1;

        curr += 2;
        return temp;
    }
    else if (regex == "." || regex == "|" || regex == "*")
    {
        return temp;
    }

    vector<string> segments = parseSegments(regex);
    stack<NFA> nfaList;
    stack<string> operatorList;

    for (size_t i = 0; i < segments.size(); i++)
    {
        string seg = segments[i];
        if (seg != "." && seg != "|" && seg != "*")
        {
            NFA a = createNFA(seg, curr);
            nfaList.push(a);
        }
        else if (seg == "*")
        {
            NFA process = nfaList.top();
            nfaList.pop();

            process.states[process.states.size() - 1].isFinal = false;
            process.states[process.states.size() - 1].transitions['$'].push_back(process.startState);

            int starti = curr;
            int endi = curr + 1;

            process.states.push_back(State());
            process.states.push_back(State());

            int s = process.states.size();

            process.states.back().isFinal = true;
            process.states[s - 2].transitions['$'].push_back(endi);
            process.states[s - 2].transitions['$'].push_back(process.startState);
            process.states[s - 3].transitions['$'].push_back(endi);

            process.startState = starti;
            process.endState = endi;

            curr += 2;

            nfaList.push(process);
        }
        else if (seg == ".")
        {
            operatorList.push(".");
            continue;
        }
        else if (seg == "|")
        {
            operatorList.push("|");
            continue;
        }
        if (!operatorList.empty() && operatorList.top() == ".")
        {
            while (!operatorList.empty() && operatorList.top() == "." && nfaList.size() >= 2)
            {
                if (i + 1 < segments.size() && segments[i + 1] == "*")
                    break;

                operatorList.pop();

                NFA right = nfaList.top();
                nfaList.pop();
                NFA left = nfaList.top();
                nfaList.pop();

                int leftLast = left.states.size() - 1;
                left.states[leftLast].isFinal = false;
                left.states[leftLast].transitions['$'].push_back(right.startState);

                for (auto &s : right.states)
                {
                    left.states.push_back(s);
                }

                left.endState = right.endState;

                nfaList.push(left);
            }
        }
    }

    while (!operatorList.empty() && nfaList.size() >= 2)
    {
        if (operatorList.top() == "|")
        {
            operatorList.pop();
            NFA right = nfaList.top();
            nfaList.pop();
            NFA left = nfaList.top();
            nfaList.pop();

            left.states[left.states.size()-1].isFinal = false;
            right.states[right.states.size()-1].isFinal = false;

            State newstart = State();
            State newend = State();
            newstart.transitions['$'].push_back(left.startState);
            newstart.transitions['$'].push_back(right.startState);
            left.states[left.states.size()-1].transitions['$'].push_back(curr+1);
            right.states[right.states.size()-1].transitions['$'].push_back(curr+1);
            newend.isFinal = true;


            for(auto &s : right.states){
                left.states.push_back(s);
            }

            left.states.push_back(newstart);
            left.states.push_back(newend);

            left.startState = curr;
            left.endState = curr + 1;

            curr += 2;

            nfaList.push(left);

        }
        else
        {
            operatorList.pop();
        }
    }

    for(char i: regex){
        if(isalnum(i)){
            nfaList.top().alphabet.insert(i);
        }
    }
    if (nfaList.empty()) return temp; 
    return nfaList.top();
}

set<int> epsilonClosure(const NFA& nfa, const set<int>& s) {
    set<int> c = s;
    queue<int> q;
    for (int x : s) q.push(x);

    while (!q.empty()) {
        int u = q.front(); 
        q.pop();
        if (nfa.states[u].transitions.count('$')) {
            for (int v : nfa.states[u].transitions.at('$')) {
                if (!c.count(v)) {
                    c.insert(v);
                    q.push(v);
                }
            }
        }
    }
    return c;
}

vector<pair<int,int>> findMatches(const NFA& nfa, const string& s) {
    vector<pair<int,int>> matches;

    for (int i = 0; i < (int)s.size(); i++) {

        set<int> cur = epsilonClosure(nfa, {nfa.startState});

        for (int j = i; j < (int)s.size(); j++) {

            set<int> nxt;
            for (int u : cur) {
                if (nfa.states[u].transitions.count(s[j])) {
                    for (int v : nfa.states[u].transitions.at(s[j])) 
                        nxt.insert(v);
                }
            }
            if (nxt.empty()) break;

            cur = epsilonClosure(nfa, nxt);

            for (int u : cur) {
                if (nfa.states[u].isFinal) {
                    matches.push_back({i, j});
                }
            }
        }
    }

    return matches;
}

string wrapMatches(const string& s, vector<pair<int,int>> matches) {
    if (matches.empty()) return s;

    sort(matches.begin(), matches.end());
    vector<pair<int,int>> merged;

    for (auto& m : matches) {
        if (merged.empty() || m.first > merged.back().second + 1) {
            merged.push_back(m);
        } else {
            merged.back().second = max(merged.back().second, m.second);
        }
    }

    string out;
    int idx = 0;
    for (auto &p : merged) {
        int L = p.first;
        int R = p.second;

        out += s.substr(idx, L - idx);
        out += "[" + s.substr(L, R - L + 1) + "]";
        idx = R + 1;
    }
    out += s.substr(idx);

    return out;
}

void writeOutput(const string& text) {
    filesystem::create_directories("outputs");

    string base = "outputs/output";
    string extension = ".txt";
    string filename = base + extension;

    int counter = 1;
    while (filesystem::exists(filename)) {
        filename = base + to_string(counter) + extension;
        counter++;
    }

    ofstream out(filename);
    out << text;
    out.close();
}

void detectAndExportMatches(const NFA& nfa, const string& text) {
    auto matches = findMatches(nfa, text);
    string wrapped = wrapMatches(text, matches);
    writeOutput(wrapped);
}
