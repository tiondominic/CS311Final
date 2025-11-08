#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stack>
using namespace std;

struct State
{
    multimap<char, int> transitions;
    bool isFinal = false;
};

struct indexes
{
    int start;
    int end;
};

class NFA
{
public:
    vector<State> States;
    stack<indexes> index;
    void createNFA(string regex)
    {
        for (int j = 0; j < regex.length(); j++)
        {
            char i = regex[j];
            bool comparison = (i != '+' && i != '(' && i != ')' && i != '*');
            if (comparison) // concatenation
            {
                int start = States.size();
                int end = start + 1;

                States.push_back(State());
                States.push_back(State());

                States[start].transitions.insert({i, end});

                indexes newi{start, end};
                if (!index.empty())
                {
                    indexes prev = index.top();
                    index.pop();
                    States[prev.end].transitions.insert({'$', newi.start});

                    newi.start = prev.start;
                }
                index.push(newi);
            }
            if (i == '+')
            {
                if (i < regex.length())
                {
                    indexes left = index.top();
                    index.pop();
                    createNFA(regex.substr(i + 1, regex.length()));
                };
                
            }
        }
        if (!index.empty())
        {
            int finalIndex = index.top().end;
            States[finalIndex].isFinal = true;
        }
    };

    vector<State> getnfa()
    {
        return States;
    }

    void epsilonClosure(const vector<State> &NFA, set<int> &states)
    {
        bool changed = true;
        while (changed)
        {
            changed = false;
            set<int> newStates = states;

            for (int s : states)
            {
                auto range = NFA[s].transitions.equal_range('$');
                for (auto it = range.first; it != range.second; ++it)
                {
                    int next = it->second;
                    if (newStates.insert(next).second)
                    {
                        changed = true;
                    }
                }
            }

            states = newStates;
        }
    }

    bool CheckIfValid(vector<State> NFA, string w)
    {
        set<int> currentStates;
        currentStates.insert(0);

        epsilonClosure(NFA, currentStates);

        for (char symbol : w)
        {
            set<int> nextStates;

            for (int s : currentStates)
            {
                auto range = NFA[s].transitions.equal_range(symbol);

                for (auto it = range.first; it != range.second; ++it)
                {
                    int go = it->second;
                    nextStates.insert(go);
                }
            }

            currentStates = nextStates;
            epsilonClosure(NFA, currentStates);
        }

        for (int s : currentStates)
        {
            if (NFA[s].isFinal)
                return true;
        }

        return false;
    }
};

int main()
{
    NFA newNFA = NFA();
    string input;
    cout << "Enter RegEx (No Spaces): ";
    cin >> input;

    newNFA.createNFA(input);
    auto nfa = newNFA.getnfa();
    string w;
    cout << "Enter String to validate: ";
    cin >> w;
    bool accepts = newNFA.CheckIfValid(nfa, w);
    cout << (accepts ? "Accepted" : "Rejected");

    return 0;
}