#include <queue>
#include <vector>
#include <tuple>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "approximate.h"
#include "regextonfa.h"
using namespace std;


string traceApproxMatch(const NFA &nfa, const string &s, int kmax)
{
    string out = "=== DNA SEARCH ===\n";
    int L = s.size();

    for (int start = 0; start < L; ++start)
    {
        int pos = start;
        int state = nfa.startState;
        int err = 0;
        bool stopped = false;

        out += "Match at index " + to_string(start) + ":\n";

        while (pos <= L && !stopped)
        {
            bool moved = false;

            if (pos < L && nfa.states[state].transitions.count(s[pos]))
            {
                int nxt = nfa.states[state].transitions.at(s[pos])[0];
                out += nfa.states[state].symbol + " --" + s[pos] + "--> " + nfa.states[nxt].symbol + "\n";
                state = nxt;
                pos++;
                moved = true;
                continue;
            }

            if (nfa.states[state].transitions.count('$'))
            {
                int nxt = nfa.states[state].transitions.at('$')[0];
                out += nfa.states[state].symbol + " --$--> " + nfa.states[nxt].symbol + "\n";
                state = nxt;
                moved = true;
                continue;
            }

            if (err < kmax)
            {
                bool hasForced = false;
                for (auto &kv : nfa.states[state].transitions)
                {
                    char a = kv.first;
                    if (a == '$') continue;

                    int nxt = kv.second[0];
                    out += nfa.states[state].symbol + " --?--> " + nfa.states[nxt].symbol + "\n";
                    state = nxt;
                    pos++;
                    err++;
                    hasForced = true;
                    moved = true;
                    break;
                }
                if (hasForced) continue;
            }

            if (!moved)
            {
                if (pos < L)
                    out += "No valid transition for '" + string(1, s[pos]) + "', stopping.\n";
                stopped = true;
            }
        }

        if (nfa.states[state].isFinal)
            out += nfa.states[state].symbol + " --> accepting state\n\n";
    }

    return out;
}
