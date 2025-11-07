#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

struct State
{
    multimap<char, int> transitions; // Problem when States have multiple transitions on same input ex. a->State1, a->state 2 while in State0
    bool isFinal = false;
};


vector<State> regexToNFA(string regex)
{
    vector<State> States;

    States.push_back(State());
    for (int i = 0; i < regex.length(); i++)
    {
        char symbol = regex[i];
        int curr = 0;

        if (symbol == '+')
        {
            States[curr].isFinal = true;
            curr = 0;
            continue;
        }

        States.push_back(State());
        if (curr == 0)
        {
            States[curr].transitions.insert({'$', curr+1});
            States.push_back(State());
            curr++;
        }
        if (symbol == '*')
        {
            States[curr].transitions.insert({'$', curr - 1});
            curr--;
            States[curr].transitions.insert({'$', curr + 1});
            curr++;
        }
        else
        {
            States[curr].transitions.insert({symbol, States.size()-1});
            curr++;
        }

        if (i == regex.length() - 1)
        {
            States.push_back(State());
            States[curr].transitions.insert({'$', States.size()-1});
            curr++;
            States[curr].isFinal = true;
        }
    }

    return States;
}

void printNFA(const vector<State> &nfa)
{
    cout << "\n=== NFA States ===\n";
    for (int i = 0; i < nfa.size(); ++i)
    {
        cout << "State " << i;
        if (nfa[i].isFinal)
            cout << " (final)";
        cout << ":\n";

        for (auto &t : nfa[i].transitions)
        {
            cout << "  --" << t.first << "--> " << t.second << "\n";
        }
        cout << endl;
    }
}

void epsilonClosure(const vector<State> &nfa, set<int> &states) // chatgpt code
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        set<int> newStates = states;
        for (int s : states)
        {
            auto range = nfa[s].transitions.equal_range('$');
            for (auto it = range.first; it != range.second; ++it)
            {
                if (newStates.insert(it->second).second)
                    changed = true;
            }
        }
        states = newStates;
    }
}

bool simulateNFA(const vector<State> &nfa, const string &input) // chatgpt code
{
    set<int> currentStates = {0};
    epsilonClosure(nfa, currentStates);

    for (char c : input)
    {
        set<int> nextStates;
        for (int s : currentStates)
        {
            auto range = nfa[s].transitions.equal_range(c);
            for (auto it = range.first; it != range.second; ++it)
            {
                nextStates.insert(it->second);
            }
        }
        currentStates = nextStates;
        epsilonClosure(nfa, currentStates);
    }

    for (int s : currentStates)
    {
        if (nfa[s].isFinal)
            return true;
    }
    return false;
}

int main()
{
    string DNA = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"; // test sample
    string RegularExpression;

    cout << "Enter a RegEx (no spaces): ";
    cin >> RegularExpression;

    vector<State> compiled = regexToNFA(RegularExpression);
    printNFA(compiled);
    // cout << compiled.size();

    string inputs;

    cout << "Enter String for testing: ";
    cin >> inputs;
    // testString(compiled, inputs);
    if (simulateNFA(compiled, inputs))
    {
        cout << "input string is accepted";
    }
    else
    {
        cout << "input string is rejected";
    }

    return 0;
}