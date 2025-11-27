#ifndef APPROXIMATE_H
#define APPROXIMATE_H

#include <string>
#include "regextonfa.h"

bool validateApprox(const NFA &nfa, const std::string &s, int maxErrors);

#endif