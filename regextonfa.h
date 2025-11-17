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

string expandCharClasses(const string &regex);
NFA createNFA(const string &regex, int &curr);
set<int> epsilonClosure(const NFA& nfa, set<int> states);
bool validateString(const NFA& nfa, const string& s);

#endif
