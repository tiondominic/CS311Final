#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

// IDEA Just like in the regex to NFA this function splits the string of XML document into iterable parts
// Then these parts are then added to an automaton 
// with respect to precedence as well

vector<string> parseXML(string XML){
    vector<string> parsed;
    stack<char> operators;

    for(char i: XML){
        if(i == '<')
        {
            operators.push('<');
        }

        if(!operators.empty() && operators.top() == '<'){
            string subs += i;
        }
        else if {

        }

    }




    return parsed;
}