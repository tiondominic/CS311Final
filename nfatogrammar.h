#ifndef NFATOGRAMMAR_H
#define NFATOGRAMMAR_H

#include <iostream>
#include <map>
#include <vector>
#include "regextonfa.h"


map<string, vector<string>> convert(NFA nfa);
void pproductions(map<string, vector<string>> prod);


#endif