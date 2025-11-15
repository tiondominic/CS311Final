#include <iostream>
#include "regextonfa.h"
using namespace std;

set<int> epsilonClosure(const NFA& nfa, set<int> states) { // NOT MODIFIED YET
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



int main(){

    return 0;
}