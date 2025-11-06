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
        char symbol = regex[i];
        if(symbol == '+'){
            States[curr].isFinal = true;
            curr = 0;
            continue;
        }

        States.push_back(State());
        if (curr == 0)
        {
            States[curr].transitions.insert({'$', curr + 1});
            States.push_back(State());
            curr++;
        }
        // else if(symbol == '*'){
        //     curr--;
        //     States[curr].transitions.insert({'$', curr-1}); need to change struct State to handle multiple transitions
        //     curr++;
        // }

        States[curr].transitions.insert({symbol, curr+1});
        curr++;

        if (i == regex.length() - 1)
        {
            States.push_back(State());
            States[curr].transitions.insert({'$', curr+1});
            curr++;
            States[curr].isFinal = true;
        }
    }

    return States;
}


void epsilonClosure(const vector<State> &states, set<int> &currStates)
{ // chatgpt code
    bool changed;
    do
    {
        changed = false;
        set<int> newStates = currStates;
        for (int s : currStates)
        {
            auto it = states[s].transitions.find('$');
            if (it != states[s].transitions.end())
            {
                int next = it->second;
                if (!currStates.count(next))
                {
                    newStates.insert(next);
                    changed = true;
                }
            }
        }
        currStates = newStates;
    } while (changed);
}

bool testString(const vector<State> &states, const string &input)
{ // chatgpt code
    set<int> currStates = {0};
    epsilonClosure(states, currStates);

    for (char c : input)
    {
        set<int> nextStates;
        for (int s : currStates)
        {
            auto it = states[s].transitions.find(c);
            if (it != states[s].transitions.end())
            {
                nextStates.insert(it->second);
            }
        }
        currStates = nextStates;
        epsilonClosure(states, currStates);
    }

    for (int s : currStates)
    {
        if (states[s].isFinal)
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

    string inputs;

    cout << "Enter String for testing: ";
    cin >> inputs;
    // testString(compiled, inputs);
    if (testString(compiled, inputs))
    {
        cout << "input string is accepted";
    }
    else
    {
        cout << "input string is rejected";
    }

    return 0;
}