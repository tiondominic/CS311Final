#ifndef NFATOGRAMMAR_H
#define NFATOGRAMMAR_H

#include <iostream>
#include <map>
#include <vector>
#include "regextonfa.h"
using namespace std;

void dfs(NFA &nfa, int start, vector<bool> &visited, int subscript);
void BFS(NFA &nfa, int start);
void convert(NFA &nfa);
void pproductions(map<string, vector<string>> prod);


#endif