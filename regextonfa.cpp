#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <cctype>
#include "regextonfa.h"
using namespace std;

vector<string> parseSegments(string regExp)
{
    int count = 0;
    string temp = "";
    vector<string> seg;
    stack<string> operators;

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

NFA createNFA(string regex, int &curr)
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
    else if (regex == "." && regex == "|" && regex == "*")
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

            nfaList.push(process); // update for more streamline
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
        // [a . a | . b . b]
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

set<int> epsilonClosure(const NFA& nfa, set<int> states) {
    set<int> closure = states;
    queue<int> q;
    for (int state : states) {
        q.push(state);
    }
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (nfa.states[current].transitions.count('$')) {
            for (int next : nfa.states[current].transitions.at('$')) {
                if (closure.find(next) == closure.end()) {
                    closure.insert(next);
                    q.push(next);
                }
            }
        }
    }
    return closure;
}

bool validateString(const NFA& nfa, const string& s) {
    set<int> currentStates = epsilonClosure(nfa, {nfa.startState});
    
    for (char c : s) {
        set<int> nextStates;
        for (int state : currentStates) {
            if (nfa.states[state].transitions.count(c)) {
                for (int next : nfa.states[state].transitions.at(c)) {
                    nextStates.insert(next);
                }
            }
        }
        currentStates = epsilonClosure(nfa, nextStates);
    }
    
    for (int state : currentStates) {
        if (nfa.states[state].isFinal) {
            return true;
        }
    }
    return false;
}
