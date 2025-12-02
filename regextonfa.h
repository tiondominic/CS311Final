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
std::set<int> epsilonClosure(const NFA& nfa, const std::set<int>& s);

std::vector<std::pair<int,int>> findMatches(const NFA& nfa, const std::string& s);

std::string wrapMatches(const std::string& s, std::vector<std::pair<int,int>> matches);

void writeOutput(const std::string& text);

void detectAndExportMatches(const NFA& nfa, const std::string& text);

#endif
