#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <cctype>
#include "regextonfa.h"
using namespace std;

// Using a BFS for Unioned States would be better but more work to be done
// use DFS instead and work from there
// Best case we use DFS for normal searching and BFS for the Unions -- More Work to combine DFS and BFS
void dfs(NFA &nfa, int start, vector<bool> &visited, int &subscript)
{
    State &state = nfa.states[start];
    state.symbol = string("S") + to_string(subscript);
    visited[start] = true;
    subscript++;
    for (auto &s : state.transitions)
    {
        for (int i : s.second)
        {
            if (!visited[i])
            {
                dfs(nfa, i, visited, subscript);
            }
        }
    }
}

void BFS(NFA &nfa)
{
    vector<bool> visited(nfa.states.size(), false);
    queue<int> q;
    q.push(nfa.startState);
    visited[nfa.startState] = true;
    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        for (auto &s : nfa.states[curr].transitions)
        {
            for (int i : s.second)
            {
                cout << nfa.states[curr].symbol << "->" << s.first << nfa.states[i].symbol << endl;
                if (!visited[i])
                {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        if (nfa.states[curr].isFinal)
        {
            cout << nfa.states[curr].symbol << "->" << "$" << endl;
        }
    }
}

void convert(NFA &nfa)
{
    vector<bool> visited(nfa.states.size(), false);
    int start = nfa.startState;
    int subscript = 0;

    dfs(nfa, start, visited, subscript);
    BFS(nfa);
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