#include <iostream>
#include <map>
#include <vector>
#include "regextonfa.h"
using namespace std;

map<string, vector<string>> convert(NFA nfa)
{
    map<string, vector<string>> productions;

    for (size_t i = 0; i < nfa.states.size(); i++)
    {
        State state = nfa.states[i];
        string nonter = "S" + to_string(i);

        if (state.transitions.empty())
        {
            productions[nonter].push_back("$");
            continue;
        }

        for (auto const &j : state.transitions)
        {
            if (!j.second.empty())
            {
                for (int k : j.second)
                {
                    if (j.first == '$')
                    {
                        productions[nonter].push_back("S" + to_string(k));
                    }
                    else
                    {
                        productions[nonter].push_back(j.first + ("S" + to_string(k)));
                    }
                }
            }
            else
            {
                productions[nonter].push_back("$");
            }
        }
    }

    return productions;
}

void pproductions(map<string, vector<string>> prod)
{
    for (auto const &a : prod)
    {
        string nvar = a.first;
        cout << nvar << "->";
        for (auto const &i : a.second)
        {
            cout << i << "|";
        }
        cout << endl;
    }
}