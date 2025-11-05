#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

struct State{
    map<char, int> transitions; // Problem when States have multiple transitions on same input ex. a->State1, a->state 2 while in State0
    bool isFinal = false;

};


vector<State> regexToNFA(string regex){
    vector<State> States;
    
    States.push_back(State());
    int curr = 0;
    for(int i=0; i < regex.length(); i++){
        State current=States[curr];
        char symbol = regex[i];

        States.push_back(State());
        curr++;
        current.transitions.insert({symbol, curr});
        States[i] = current;

        if(i == regex.length()-1){
            States[curr].isFinal = true;
        }
    }

    return States;
}

bool Test(vector<State> nfa, string input) { //CHATGPT CODE FOR TESTING THE NFA
    int current = 0;

    for (char c : input) {
        auto it = nfa[current].transitions.find(c);
        if (it == nfa[current].transitions.end()) {
            return false; // no valid transition for this symbol
        }
        current = it->second; // move to next state
    }

    return nfa[current].isFinal; // accept if final
}
int main(){
    string DNA = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"; // test sample
    string RegularExpression;

    cout << "Enter a RegEx: ";
    cin >> RegularExpression;

    vector<State> compiled = regexToNFA(RegularExpression);

    string inputs;

    cout << "Enter String for testing: ";
    cin >> inputs;
    if(Test(compiled, inputs)){
        cout << "input string is accepted";
    }
    else{
        cout << "input string is rejected";
    }

    return 0;
}