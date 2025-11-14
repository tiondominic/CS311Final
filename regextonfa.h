#ifndef REGEXTONFA_H
#define REGEXTONFA_H
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <cctype>
using namespace std;


struct State
{
    map<char, vector<int>> transitions;
    bool isFinal = false;
};

struct NFA
{
    vector<State> states;
    set<char> alphabet;
    int startState;
    int endState;
};

vector<string> parseSegments(string regExp);
NFA createNFA(string regex, int &curr);
set<int> epsilonClosure(const NFA& nfa, set<int> states);
bool validateString(const NFA& nfa, const string& s);



#endif