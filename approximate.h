#ifndef APPROXIMATE_H
#define APPROXIMATE_H

#include <string>
#include "regextonfa.h"

struct MatchSpan {
    int start;
    int end;
};

void runApproxSearchAndWrite(const NFA &nfa, const string &input, int kmax);

#endif