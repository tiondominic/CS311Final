#include <iostream>
#include <string>
#include "regextonfa.h"
#include "nfatogrammar.h"
using namespace std;


int main()
{
    cout << "Enter Regex: ";
    string regex;
    getline(cin, regex);

    int a = 0;
    NFA nfa = createNFA(regex, a);

    cout << "Enter String: ";
    string w;
    getline(cin, w);
    cout << (validateString(nfa, w) ? "Accepted" : "Rejected") << endl;


    cout << "Alphabet: ";
    for(char i: nfa.alphabet){
        cout << i << " ";
    }
    cout << endl;

    map<string, vector<string>> productions = convert(nfa);
    pproductions(productions);
    
    

    return 0;
}