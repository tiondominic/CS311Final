#ifndef REGEXTONFA_H
#define REGEXTONFA_H

#include <vector>
#include <string>
#include <set>
#include <map>
using namespace std;

struct State {
    map<char, vector<int>> transitions;
    bool isFinal = false;
    string symbol;
};

struct NFA {
    vector<State> states;
    set<char> alphabet;
    int startState = -1;
    int endState = -1;
};

NFA createNFA(const string &regex, int &curr);
set<int> epsilonClosure(const NFA& nfa, const set<int>& s);

string traceTransitions(const NFA &nfa, const string &input, int &i);

string findDNAPatterns(const NFA &nfa, const string &dna);

void writeOutput(const string& text, const string &folder);


#endif
