#include "converter.h"
#include <queue>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void dfs(NFA &n, int s, vector<bool> &v, int &i)
{
    v[s] = true;
    n.states[s].symbol = "S" + to_string(i++);
    for (auto &kv : n.states[s].transitions)
        for (int nx : kv.second)
            if (!v[nx])
                dfs(n, nx, v, i);
}

string GrammarAndTransition(NFA &n)
{
    string output;
    string outputGrammar = "=== REGULAR GRAMMAR===\n";
    string outputTransition = "\n=== TRANSITIONS ===\n";
    vector<bool> v(n.states.size(), false);
    queue<int> q;
    q.push(n.startState);
    v[n.startState] = true;

    outputTransition += "Start State: " + n.states[n.startState].symbol + "\n";
    outputTransition += "Final State: " + n.states[n.endState].symbol + "\n";

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto &kv : n.states[u].transitions)
        {
            outputTransition += "(" + n.states[u].symbol + ", " + kv.first + ") -> {";
            for (int nx : kv.second)
            {
                outputGrammar += n.states[u].symbol + "->" + kv.first + n.states[nx].symbol + "\n";
                outputTransition += n.states[nx].symbol + ", ";

                if (!v[nx])
                {
                    v[nx] = true;
                    q.push(nx);
                }
            }

            outputTransition += "}\n";
        }
        if (n.states[u].isFinal)
            outputGrammar += n.states[u].symbol + "->$\n";
    }
    outputTransition += "Alphabet: ";
    for (char c : n.alphabet)
    {
        outputTransition += c;
        outputTransition += ' ';
    }
    outputTransition += "\n";

    output = outputGrammar + outputTransition;
    return output;
}

string convert(NFA &n)
{
    vector<bool> v(n.states.size(), false);
    int id = 0;
    dfs(n, n.startState, v, id);
    string output = GrammarAndTransition(n);

    return output;
}
