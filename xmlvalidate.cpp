#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

// IDEA Just like in the regex to NFA this function splits the string of XML document into iterable parts
// Then these parts are then added to an automaton 
// with respect to precedence as well

string removespace(const string& XML){
    string temp = "";
    for(char i: XML){
        if(i != ' '){
            temp += i;
        }
    }
    return temp;
}

vector<string> parseXML(const string& a){
    string XML = removespace(a);
    vector<string> parsed;
    stack<char> operators;
    string subs = "";
    

    for(size_t c=0; c < XML.length(); c++){
        char i = XML[c];
        if(i == '<')
        {
            operators.push('<');
            continue;
        }

        if(i == '>') {

            operators.pop();
            parsed.push_back(subs);
            subs = "";
            continue;
        }
        
        subs += i;
        if(operators.empty() && XML[c+1] == '<'){
            parsed.push_back(subs);
            subs = "";
            
        }

    }

    return parsed;
}