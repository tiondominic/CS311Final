#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

struct State
{
    map<char, int> transitions; // Problem when States have multiple transitions on same input ex. a->State1, a->state 2 while in State0
    bool isFinal = false;
};

vector<State> regexToNFA(string regex)
{
    vector<State> States;

    States.push_back(State());
    int curr = 0;
    for (int i = 0; i < regex.length(); i++)
    {
        State current = States[curr];
        char symbol = regex[i];

        if (i + 1 < regex.length() && regex[i + 1] == '*')
        {
            int temp = curr;
            States.push_back(State());
            States[curr].transitions.insert({'$', ++temp});
            States[temp].transitions.insert({symbol, temp});
            States.push_back(State());
            States[++curr].transitions.insert({'$', ++temp});

            i++;
        }
        else
        {
            States.push_back(State());
            curr++;
            current.transitions.insert({symbol, curr});
            States[i] = current;
        }

        if (i == regex.length() - 1)
        {
            States[curr].isFinal = true;
        }
    }

    return States;
}

void epsilonClosure(const vector<State> &nfa, set<int> &states) //CHATGPT CODE FOR TESTING
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        set<int> newStates = states;
        for (int s : states)
        {
            auto it = nfa[s].transitions.find('$');
            if (it != nfa[s].transitions.end())
            {
                int next = it->second;
                if (newStates.find(next) == newStates.end())
                {
                    newStates.insert(next);
                    changed = true;
                }
            }
        }
        states = newStates;
    }
}

bool Test(const vector<State> &nfa, const string &input) // CHATGPT CODE FOR TESTING
{
    set<int> currentStates = {0};       // start from state 0
    epsilonClosure(nfa, currentStates); // include ε-reachable states before reading input

    for (char c : input)
    {
        set<int> nextStates;

        for (int s : currentStates)
        {
            auto it = nfa[s].transitions.find(c);
            if (it != nfa[s].transitions.end())
            {
                nextStates.insert(it->second);
            }
        }

        if (nextStates.empty())
            return false;

        epsilonClosure(nfa, nextStates); // handle ε-moves after each symbol
        currentStates = nextStates;
    }

    // Check if any active state is final
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

    cout << "Enter a RegEx: ";
    cin >> RegularExpression;

    vector<State> compiled = regexToNFA(RegularExpression);

    string inputs;

    cout << "Enter String for testing: ";
    cin >> inputs;
    if (Test(compiled, inputs))
    {
        cout << "input string is accepted";
    }
    else
    {
        cout << "input string is rejected";
    }

    return 0;
}