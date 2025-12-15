#ifndef MINIMIZE_H
#define MINIMIZE_H

#include <vector>
#include <set>
#include "regextonfa.h"
using namespace std;


struct dfaState {
    map<char, vector<int>> transitions;
    bool isFinal = false;
    string symbol;
};



#endif