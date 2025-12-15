#ifndef APPROXIMATE_H
#define APPROXIMATE_H

#include <string>
#include "regextonfa.h"


string traceApproxMatch(const NFA &nfa, const string &s, int kmax);

#endif