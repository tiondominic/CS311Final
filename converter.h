#ifndef CONVERTER_H
#define CONVERTER_H
#include <vector>
#include <bits/stdc++.h>
#include "regextonfa.h"
using namespace std;

void writeOutput(const string &text);
void dfs(NFA&,int,vector<bool>&,int&);
string GrammarAndTransition(NFA&);
string convert(NFA&);

#endif
