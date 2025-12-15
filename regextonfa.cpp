#include "regextonfa.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <bits/stdc++.h>
using namespace std;

string expandCharClasses(const string &regex)
{
    string out;
    size_t i = 0;
    while (i < regex.size())
    {
        char c = regex[i];
        if (c == '\\')
        {
            if (i + 1 < regex.size())
            {
                out += regex.substr(i, 2);
                i += 2;
            }
            else
                out += regex[i++];
        }
        else if (c == '[')
        {
            size_t j = i + 1;
            while (j < regex.size() && !(regex[j] == ']' && regex[j - 1] != '\\'))
                j++;
            if (j == regex.size())
            {
                out += '[';
                i++;
                continue;
            }

            string content = regex.substr(i + 1, j - (i + 1));
            vector<char> symbols;
            for (size_t k = 0; k < content.size(); k++)
            {
                if (content[k] == '\\' && k + 1 < content.size())
                {
                    symbols.push_back(content[++k]);
                }
                else if (k + 2 < content.size() && content[k + 1] == '-')
                {
                    char a = content[k], b = content[k + 2];
                    if (a <= b)
                        for (char x = a; x <= b; x++)
                            symbols.push_back(x);
                    else
                        for (char x = a; x >= b; x--)
                            symbols.push_back(x);
                    k += 2;
                }
                else
                    symbols.push_back(content[k]);
            }
            out.push_back('(');
            for (size_t k = 0; k < symbols.size(); k++)
            {
                out.push_back(symbols[k]);
                if (k + 1 < symbols.size())
                    out.push_back('|');
            }
            out.push_back(')');
            i = j + 1;
        }
        else
            out += regex[i++];
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

            left.states[left.states.size() - 1].isFinal = false;
            right.states[right.states.size() - 1].isFinal = false;

            State newstart = State();
            State newend = State();
            newstart.transitions['$'].push_back(left.startState);
            newstart.transitions['$'].push_back(right.startState);
            left.states[left.states.size() - 1].transitions['$'].push_back(curr + 1);
            right.states[right.states.size() - 1].transitions['$'].push_back(curr + 1);
            newend.isFinal = true;

            for (auto &s : right.states)
            {
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

    for (char i : regex)
    {
        if (isalnum(i))
        {
            nfaList.top().alphabet.insert(i);
        }
    }
    if (nfaList.empty())
        return temp;
    return nfaList.top();
}

set<int> epsilonClosure(const NFA &nfa, const set<int> &s)
{
    set<int> c = s;
    queue<int> q;
    for (int x : s)
        q.push(x);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        if (nfa.states[u].transitions.count('$'))
        {
            for (int v : nfa.states[u].transitions.at('$'))
            {
                if (!c.count(v))
                {
                    c.insert(v);
                    q.push(v);
                }
            }
        }
    }
    return c;
}


string traceTransitions(const NFA &nfa, const string &input, int &i) {
    string output;
    set<pair<int,int>> printedTransitions;
    set<int> currentStates = { nfa.startState };
    queue<int> q;

    for (int s : currentStates) q.push(s);
    set<int> visited;
    while (!q.empty()) {
        int s = q.front(); q.pop();
        if (!visited.insert(s).second) continue;

        if (nfa.states[s].transitions.count('$')) {
            for (int t : nfa.states[s].transitions.at('$')) {
                if (printedTransitions.insert({s,t}).second) {
                    output += nfa.states[s].symbol + " --$--> " + nfa.states[t].symbol + "\n";
                }
                q.push(t);
                currentStates.insert(t);
            }
        }
    }

    int consumed = 0;

    for (char c : input) {
        set<int> nextStates;

        for (int s : currentStates) {
            if (nfa.states[s].transitions.count(c)) {
                for (int t : nfa.states[s].transitions.at(c)) {
                    if (printedTransitions.insert({s,t}).second) {
                        output += nfa.states[s].symbol + " --" + c + "--> " + nfa.states[t].symbol + "\n";
                    }
                    nextStates.insert(t);
                }
            }
        }

        if (nextStates.empty()) {
            output += "No valid transition for '" + string(1, c) + "', stopping.\n";
            break;
        }

        q = queue<int>();
        visited.clear();
        for (int s : nextStates) q.push(s);

        while (!q.empty()) {
            int s = q.front(); q.pop();
            if (!visited.insert(s).second) continue;

            if (nfa.states[s].transitions.count('$')) {
                for (int t : nfa.states[s].transitions.at('$')) {
                    if (printedTransitions.insert({s,t}).second) {
                        output += nfa.states[s].symbol + " --$--> " + nfa.states[t].symbol + "\n";
                    }
                    q.push(t);
                    nextStates.insert(t);
                }
            }
        }

        currentStates = nextStates;
        consumed++;
    }

    bool accepted = false;
    for (int s : currentStates) {
        if (nfa.states[s].isFinal) {
            accepted = true;
            output += nfa.states[s].symbol + " --> accepting state\n";
        }
    }

    if (!accepted) output += "Input rejected.\n";

    i += consumed;
    return output;
}

string findDNAPatterns(const NFA &nfa, const string &dna)
{
    vector<string> results;
    string output = "\n=== DNA SEARCH ===\n";
    int i = 0;

    for (; i < dna.size(); i++)
    {
        string substring = dna.substr(i);
        string trace = traceTransitions(nfa, substring, i);

        if (trace.find("accepting state") != string::npos)
        {
            results.push_back("Match at index " + to_string(i) + ":\n" + trace);
        }
    }

    for(string s: results){
        output += s + "\n";
    }

    return output;
}

void writeOutput(const string &text, const string &folder)
{
    filesystem::create_directories(folder);

    string base = folder + "/output";
    string extension = ".txt";
    string filename = base + extension;

    int counter = 1;
    while (filesystem::exists(filename))
    {
        filename = base + to_string(counter) + extension;
        counter++;
    }

    ofstream out(filename);
    out << text;
    out.close();
}

