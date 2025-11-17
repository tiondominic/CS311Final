#include <iostream>
#include <string>
#include "regextonfa.h"
#include "nfatogrammar.h"
#include "approximate.h"
#include "xmlvalidate.h"

using namespace std;

void waitForKey() {
    cout << "\nPress ENTER to continue...";
    cin.ignore();
}

void runRegexExact() {
    cout << "\n=== REGEX TO NFA (Exact Matching) ===\n";

    string regex;
    cout << "Enter Regex: ";
    getline(cin, regex);

    int a = 0;
    NFA nfa = createNFA(regex, a);

    string w;
    cout << "Enter String to validate: ";
    getline(cin, w);

    cout << (validateString(nfa, w) ? "ACCEPTED\n" : "REJECTED\n");

    cout << "\nAlphabet: ";
    for (char c : nfa.alphabet) cout << c << " ";
    cout << "\n\nGrammar Productions:\n";
    convert(nfa);

    waitForKey();
}

void runApproximate() {
    cout << "\n=== APPROXIMATE MATCHING ===\n";

    string regex;
    cout << "Enter Regex: ";
    getline(cin, regex);

    int a = 0;
    NFA nfa = createNFA(regex, a);

    string s;
    cout << "Enter String to test: ";
    getline(cin, s);

    int k;
    cout << "Enter allowed errors (k): ";
    cin >> k;
    cin.ignore();

    bool ok = validateApprox(nfa, s, k);
    cout << (ok ? "MATCHED WITH APPROXIMATION\n" : "NOT MATCHED\n");

    waitForKey();
}

string readMultilineXML() {
    cout << "Enter XML (finish with a blank line):\n";
    string xml, line;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        xml += line;
    }
    return xml;
}

void runXMLValidation() {
    cout << "\n=== XML VALIDATION (PDA) ===\n";

    string xml = readMultilineXML();
    bool valid = validateXML(xml, true);

    cout << (valid ? "\nXML IS VALID\n" : "\nXML IS INVALID\n");

    waitForKey();
}

int main() {
    while (true) {
        cout << "\n=============================\n";
        cout << "         MAIN MENU\n";
        cout << "=============================\n";
        cout << "1. Regex to NFA (Exact Matching)\n";
        cout << "2. Approximate Matching\n";
        cout << "3. XML Validation using PDA\n";
        cout << "4. Exit\n";
        cout << "Your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) runRegexExact();
        else if (choice == 2) runApproximate();
        else if (choice == 3) runXMLValidation();
        else if (choice == 4) break;
        else cout << "Invalid choice.\n";
    }

    return 0;
}
