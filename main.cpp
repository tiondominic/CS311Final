#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
using namespace std;

struct State
{
    map<char, vector<int>> transitions;
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

    void regexToNFA(string regex)
    {
        regex = insertConcat(regex);
        stack<char> ops;

        for (int i = 0; i < regex.length(); i++)
        {
            char c = regex[i];

            if (isalnum(c))
            {
                int start = States.size();
                int end = start + 1;

                States.push_back(State());
                States.push_back(State());

                States[start].transitions[c].push_back(end);
                index.push({start, end});
            }
            else if (c == '(')
                ops.push(c);
            else if (c == ')')
            {
                while (!ops.empty() && ops.top() != '(')
                {
                    processOperator(ops.top());
                    ops.pop();
                }
                if (!ops.empty())
                    ops.pop();
            }
            else
            {
                while (!ops.empty() && precedence(ops.top()) >= precedence(c))
                {
                    processOperator(ops.top());
                    ops.pop();
                }
                ops.push(c);
            }
        }

        while (!ops.empty())
        {
            processOperator(ops.top());
            ops.pop();
        }

        if (!index.empty())
        {
            States[index.top().end].isFinal = true;
        }
    }

    vector<State> getnfa() { return States; }

private:
    int precedence(char c)
    {
        if (c == '*')
            return 3;
        if (c == '.')
            return 2;
        if (c == '+')
            return 1;
        return 0;
    }

    string insertConcat(const string &regex)
    {
        string output;
        for (int i = 0; i < regex.size(); i++)
        {
            char c1 = regex[i];
            output += c1;
            if (i + 1 < regex.size())
            {
                char c2 = regex[i + 1];
                if ((isalnum(c1) || c1 == '*' || c1 == ')') &&
                    (isalnum(c2) || c2 == '('))
                {
                    output += '.';
                }
            }
        }
        return output;
    }

    void processOperator(char op)
    {
        if (op == '*')
        { 
            indexes f = index.top();
            index.pop();
            int start = States.size();
            int end = start + 1;

            States.push_back(State());
            States.push_back(State());

            States[start].transitions['$'].push_back(f.start);
            States[start].transitions['$'].push_back(end);
            States[f.end].transitions['$'].push_back(f.start);
            States[f.end].transitions['$'].push_back(end);

            index.push({start, end});
        }
        else if (op == '.')
        { 
            indexes f2 = index.top();
            index.pop();
            indexes f1 = index.top();
            index.pop();

            States[f1.end].transitions['$'].push_back(f2.start);
            index.push({f1.start, f2.end});
        }
        else if (op == '+')
        {
            indexes f2 = index.top();
            index.pop();
            indexes f1 = index.top();
            index.pop();
            int start = States.size();
            int end = start + 1;
            States.push_back(State());
            States.push_back(State());
            States[start].transitions['$'].push_back(f1.start);
            States[start].transitions['$'].push_back(f2.start);
            States[f1.end].transitions['$'].push_back(end);
            States[f2.end].transitions['$'].push_back(end);
            index.push({start, end});
        }
    }
};

void epsilonClosure(const vector<State> &nfa, set<int> &states) // try to understand later
{
    stack<int> s;
    for (int st : states)
        s.push(st);

    while (!s.empty())
    {
        int st = s.top();
        s.pop();

        auto it = nfa[st].transitions.find('$');
        if (it != nfa[st].transitions.end())
        {
            for (int next : it->second)
            {
                if (!states.count(next))
                {
                    states.insert(next);
                    s.push(next);
                }
            }
        }
    }
}

bool testNFA(const vector<State> &nfa, const string &input)
{
    set<int> currentStates;
    currentStates.insert(0);
    epsilonClosure(nfa, currentStates);

    for (char c : input)
    {
        set<int> nextStates;

        for (int state : currentStates)
        {
            auto it = nfa[state].transitions.find(c);
            if (it != nfa[state].transitions.end())
            {
                for (int next : it->second)
                    nextStates.insert(next);
            }
        }

        currentStates = nextStates;
        epsilonClosure(nfa, currentStates);
    }

    for (int state : currentStates)
    {
        if (nfa[state].isFinal)
            return true;
    }
    return false;
}

void function1()
{
    NFA newnfa = NFA();
    string w;
    string regex;

    cout << "Enter Regex: ";
    cin >> regex;

    newnfa.regexToNFA(regex);
    vector<State> NFA = newnfa.getnfa();

    cout << "Enter string to test: ";
    cin >> w;

    bool accepted = testNFA(NFA, w);
    cout << (accepted ? "Acceted" : "Rejected") << endl;
}

void instructions(){
    cout << "Enter a Regex of form (a+b)*" << endl;
    cout << "Spaces are not allowed!" << endl;
}

int main()
{

    bool loop = true;
    int decision;

    cout <<"1. Instructions\n2. Enter Regex\n3. Exit" << endl;
    do
    {
        cout << ">> ";
        cin >> decision;
        switch (decision)
        {case 1:
            instructions();
            break;
        case 2:
            function1();
            break;
        case 3:
            loop = false;
            break;
        default:
            cout << "Invalid" << endl;
            break;
    }

    } while (loop);


    return 0;
}