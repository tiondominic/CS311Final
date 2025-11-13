#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <stack>
using namespace std;

struct State
{
    map<char, vector<int>> transitions;
    bool isFinal = false;
};

struct NFA
{
    vector<State> states;
    int startState;
    int endState;
};

vector<string> parseSegments(string regExp)
{
    int count = 0;
    string temp = "";
    vector<string> seg;
    stack<string> operators;

    for (int i = 0; i < regExp.length(); i++)
    {
        char c = regExp[i];

        if (c == '(')
        {
            count++;
            if (count == 1)
                continue;
        }
        else if (c == ')')
        {
            count--;
            if (count == 0)
            {
                seg.push_back(temp);
                temp.clear();

                if (i < regExp.length() - 1)
                {
                    char next = regExp[i + 1];
                    if (next != '*' && next != '|' && next != ')')
                        seg.push_back(".");
                }
                continue;
            }
        }

        temp += c;
        if (count == 0)
        {
            seg.push_back(temp);
            temp.clear();

            if (i < regExp.length() - 1)
            {
                char next = regExp[i + 1];
                if (next != '*' && next != '|' && next != ')')
                    seg.push_back(".");
            }
        }
    }
    return seg;
}

NFA createNFA(string regex)
{
    NFA temp;
    static int curr = 0;
    if (regex == "#")
    {
        temp.states.push_back(State());
        return temp;
    }
    else if (regex == "")
    {
        State s;
        s.isFinal = true;
        temp.states.push_back(s);
        return temp;
    }
    else if (regex.length() == 1)
    {
        temp.states.push_back(State());
        temp.states.push_back(State());

        temp.states[0].transitions[regex[0]].push_back(curr + 1);
        temp.states[1].isFinal = true;

        temp.startState = curr;
        temp.endState = curr + 1;

        curr += 2;
        return temp;
    }
    else if (regex == "." && regex == "|" && regex == "*")
    {
        return temp;
    }

    vector<string> segments = parseSegments(regex);
    stack<NFA> nfaList;
    stack<string> operatorList;

    for (int i = 0; i < segments.size(); i++)
    {
        string seg = segments[i];
        if (seg != "." && seg != "|" && seg != "*")
        {
            NFA a = createNFA(seg);
            nfaList.push(a);
        }
        else if (seg == "*")
        {
            NFA process = nfaList.top();
            nfaList.pop();

            process.states[process.states.size() - 1].isFinal = false;
            process.states[process.states.size() - 1].transitions['$'].push_back(process.startState);

            int starti = curr;
            int endi = curr + 1;

            process.states.push_back(State());
            process.states.push_back(State());

            int s = process.states.size();

            process.states[s - 1].isFinal = true;
            process.states[s - 2].transitions['$'].push_back(endi);
            process.states[s - 2].transitions['$'].push_back(process.startState);
            process.states[s - 3].transitions['$'].push_back(endi);

            curr += 2;

            nfaList.push(process); // update for more streamline
        }
        else if (seg == ".")
        {
            operatorList.push(".");
        }
        else if (seg == "|")
        {
            operatorList.push("|");
        }
        // [a . a | . b . b]
        if (!operatorList.empty() && operatorList.top() == ".")
        {
            while (!operatorList.empty() && operatorList.top() == "." && nfaList.size() >= 2)
            {
                if (i + 1 < segments.size() && segments[i + 1] == "*")
                    break;

                operatorList.pop();

                NFA right = nfaList.top();
                nfaList.pop();
                NFA left = nfaList.top();
                nfaList.pop();

                int leftLast = left.states.size() - 1;
                left.states[leftLast].isFinal = false;
                left.states[leftLast].transitions['$'].push_back(right.startState);

                for (auto &s : right.states)
                {
                    left.states.push_back(s);
                }

                left.endState = right.endState;

                nfaList.push(left);
            }
        }
    }

    return nfaList.top();
}

set<int> epsilonClosure(const NFA &nfa, const set<int> &states)
{
    stack<int> st;
    set<int> closure = states;

    for (int s : states)
        st.push(s);

    while (!st.empty())
    {
        int current = st.top();
        st.pop();

        auto it = nfa.states[current].transitions.find('$');
        if (it != nfa.states[current].transitions.end())
        {
            for (int next : it->second)
            {
                if (closure.find(next) == closure.end())
                {
                    closure.insert(next);
                    st.push(next);
                }
            }
        }
    }

    return closure;
}

bool validateString(const NFA &nfa, const string &input)
{
    set<int> currentStates = {nfa.startState};
    currentStates = epsilonClosure(nfa, currentStates);

    cout << "Initial epsilon closure: ";
    for (int s : currentStates)
        cout << s << " ";
    cout << "\n";

    for (char symbol : input)
    {
        cout << "Processing input: '" << symbol << "'\n";
        set<int> nextStates;

        for (int state : currentStates)
        {
            cout << "Q: " << state << ", Input: " << symbol << "\n";

            auto it = nfa.states[state].transitions.find(symbol);
            if (it != nfa.states[state].transitions.end())
            {
                for (int next : it->second)
                    nextStates.insert(next);
            }
        }

        // apply epsilon closure after moving on symbol
        currentStates = epsilonClosure(nfa, nextStates);

        cout << "States after epsilon closure: ";
        for (int s : currentStates)
            cout << s << " ";
        cout << "\n";
    }

    // check if any current state is final
    for (int state : currentStates)
    {
        if (nfa.states[state].isFinal)
            return true;
    }

    return false;
}
int main()
{
    string regex;
    string w;
    cout << "Enter RegEx: ";
    getline(cin, regex);

    NFA test = createNFA(regex);

    cout << "Enter String: ";
    getline(cin, w);
    cout << "Input '" << w << "' : " << (validateString(test, w) ? "Accepted" : "Rejected") << endl;
    return 0;
}