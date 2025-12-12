#include "xmlvalidate.h"
#include <stack>
#include <iostream>
using namespace std;

static string strip(const string &x)
{
    string r;
    for (char c : x)
        if (c != ' ' && c != '\n' && c != '\t')
            r += c;
    return r;
}

vector<string> tokenizeXML(const string &in)
{
    string x = strip(in), cur;
    bool tag = false;
    vector<string> t;
    for (char c : x)
    {
        if (c == '<')
        {
            tag = true;
            cur.clear();
        }
        else if (c == '>')
        {
            tag = false;
            t.push_back(cur);
        }
        else if (tag)
            cur.push_back(c);
    }
    return t;
}

bool validateXML(const string &xml, bool dbg)
{
    vector<string> t = tokenizeXML(xml);
    stack<string> st;

    cout << "\n--- XML Validation (PDA Simulation) ---\n";

    for (auto &x : t)
    {
        if (!x.empty() && x[0] == '/')
        {
            string tag = x.substr(1);
            if (dbg)
                cout << "Read end-tag </" << tag << ">\n";
            if (st.empty())
            {
                cout << "Error: unexpected </" << tag << ">\n";
                return false;
            }
            if (st.top() != tag)
            {
                cout << "Error: expected </" << st.top() << "> got </" << tag << ">\n";
                return false;
            }
            st.pop();
            if (dbg)
                cout << "Matched </" << tag << ">\n";
        }
        else
        {
            if (dbg)
                cout << "Read start-tag <" << x << ">\n";
            st.push(x);
        }
        if (dbg)
        {
            cout << "Stack: [ ";
            stack<string> tmp = st;
            vector<string> rev;
            while (!tmp.empty())
            {
                rev.push_back(tmp.top());
                tmp.pop();
            }
            for (auto it = rev.rbegin(); it != rev.rend(); ++it)
                cout << *it << " ";
            cout << "]\n\n";
        }
    }

    if (st.empty())
    {
        cout << "XML structure is VALID.\n";
        return true;
    }
    cout << "Error: unclosed tags: ";
    while (!st.empty())
    {
        cout << st.top() << " ";
        st.pop();
    }
    cout << "\n";
    return false;
}

bool isMatchingPair(char open, char close) {
    return (open == 'A' && close == 'U') ||
           (open == 'U' && close == 'A') ||
           (open == 'C' && close == 'G') ||
           (open == 'G' && close == 'C');
}

bool validateRNA(const string& s) {
    stack<char> st;

    for (char c : s) {

        if (c == '.') {
            continue;
        }

        if (!st.empty() && isMatchingPair(st.top(), c)) {
            st.pop();
        }
        else {
            st.push(c);
        }
    }
    return st.empty();
}