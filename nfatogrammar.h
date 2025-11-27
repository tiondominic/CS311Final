#ifndef NFATOGRAMMAR_H
#define NFATOGRAMMAR_H
#include <vector>
#include "regextonfa.h"
using namespace std;

void dfs(NFA&,int,vector<bool>&,int&);
void BFS(NFA&);
void convert(NFA&);

#endif
