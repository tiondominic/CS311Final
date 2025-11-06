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

    void getData() const
    {
        cout << "isFinal: " << boolalpha << isFinal << endl;
        for (auto &t : transitions)
        {
            cout << "\b  '" << t.first << "' -> " << t.second << endl;
        }
    }
};

void print(vector<State> states)
{
    for (State i : states)
    {
        cout << "<";
        i.getData();
        cout << "\b>";
    }
}

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

            i++;
            curr++;
        }
        else
        {
            print(States);
            States.push_back(State());
            States[curr].transitions.insert({symbol, curr + 1});
            curr++;
        }

        if (i == regex.length() - 1)
        {
            States[curr].isFinal = true;
        }
    }

    return States;
}

void Test(const vector<State> &states, const string &input) //CHATGPT CODE FOR TESTING
{
    int current = 0;

    for (char symbol : input)
    {
        if (states[current].transitions.count(symbol))
        {
            current = states[current].transitions.at(symbol);
        }
        else if (states[current].transitions.count('$'))
        {
            current = states[current].transitions.at('$');
            // Retry current symbol after epsilon
            if (states[current].transitions.count(symbol))
            {
                current = states[current].transitions.at(symbol);
            }
            else
            {
                cout << "Rejected: no transition for '" << symbol << "'\n";
                return;
            }
        }
        else
        {
            cout << "Rejected: no transition for '" << symbol << "'\n";
            return;
        }
    }

    if (states[current].isFinal)
    {
        cout << "Accepted\n";
    }
    else
    {
        cout << "Rejected: reached non-final state\n";
    }
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
    Test(compiled, inputs);
    // if (Test(compiled, inputs))
    // {
    //     cout << "input string is accepted";
    // }
    // else
    // {
    //     cout << "input string is rejected";
    // }

    return 0;
}