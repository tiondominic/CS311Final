#include <iostream>
#include <map>
#include <vector>
#include "regextonfa.h"
using namespace std;

map<string, vector<string>> convert(NFA nfa)
{
    map<string, vector<string>> productions;

    for (int i = 0; i < nfa.states.size(); i++)
    {
        State state = nfa.states[i];
        for (auto const &j : state.transitions)
        {
            for (int k : j.second)
            {
                string nvar = "S" + to_string(i);
                if (j.first == '$')
                {
                    cout << "S" << i << "->" << "S" << k << endl;
                    productions[nvar].push_back("S" + to_string(k));
                }
                else
                {
                    cout << "S" << i << "->" << j.first << "S" << k << endl;
                    productions[nvar].push_back(j.first + "S" + to_string(k));
                }

            }
        }
    }

    return productions;
}
