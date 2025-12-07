#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "regextonfa.h"
#include "nfatogrammar.h"
#include "approximate.h"
#include "xmlvalidate.h"
using namespace std;

static string file_name = "None";
bool using_file = false;
static string selected_file;

void waitForKey()
{
    cout << "\nPress ENTER to continue...";
    cin.ignore();
}

void runRegexExact()
{
    cout << "\n=== REGEX TO NFA (Exact Matching) ===\n";

    string regex;
    cout << "Enter Regex: ";
    getline(cin, regex);

    int a = 0;
    NFA nfa = createNFA(regex, a);

    string w;
    if (using_file)
    {
        cout << "Using file" << endl;
        w = selected_file;
    }
    else
    {
        cout << "Enter String to validate: ";
        getline(cin, w);
    }

    detectAndExportMatches(nfa, w);

    cout << "\nAlphabet: ";
    for (char c : nfa.alphabet)
        cout << c << " ";
    cout << "\n\nGrammar Productions:\n";
    convert(nfa);

    waitForKey();
}

void runApproximate()
{
    cout << "\n=== APPROXIMATE MATCHING ===\n";

    string regex;
    cout << "Enter Regex: ";
    getline(cin, regex);

    int a = 0;
    NFA nfa = createNFA(regex, a);

    string s;
    if (using_file)
    {
        cout << "Using file" << endl;
        s = selected_file;
    }
    else
    {
        cout << "Enter String to validate: ";
        getline(cin, s);
    }

    int k;
    cout << "Enter allowed errors (k): ";
    cin >> k;
    cin.ignore();

    runApproxSearchAndWrite(nfa, s, k);

    waitForKey();
}

string readMultilineXML()
{
    cout << "Enter XML (finish with a blank line):\n";
    string xml, line;
    while (true)
    {
        getline(cin, line);
        if (line.empty())
            break;
        xml += line;
    }
    return xml;
}

void runXMLValidation()
{
    cout << "\n=== XML VALIDATION (PDA) ===\n";
    string xml;

    if (using_file)
    {
        cout << "Using file" << endl;
        xml = selected_file;
    }
    else
    {
        xml = readMultilineXML();
    }
    bool valid = validateXML(xml, true);
    cout << (valid ? "\nXML IS VALID\n" : "\nXML IS INVALID\n");

    waitForKey();
}

void validateRNA()
{
    cout << "\n=== RNA VALIDATION (PDA) ===\n";
    string rna;

    if (using_file)
    {
        cout << "Using file" << endl;
        rna = selected_file;
    }
    else
    {
        cout << "Enter RNA structure: ";
        getline(cin, rna);

    }
    bool valid = validateRNA(rna);
    cout << (valid ? "\nRNA IS VALID\n" : "\nRNA IS INVALID\n");

    waitForKey();
}

void loadfile()
{
    selected_file = "";
    filesystem::path current_dir = filesystem::current_path();
    filesystem::path test_dir = current_dir / "tests";
    vector<filesystem::directory_entry> files;

    int file_index = 0;

    try
    {
        for (const auto &entry : filesystem::directory_iterator(test_dir))
        {

            std::cout << file_index << ". " << entry.path().filename().string() << endl;

            if (entry.is_regular_file())
            {
                files.push_back(entry);
                file_index++;
            }
        }
    }
    catch (const filesystem::filesystem_error &e)
    {
        std::cerr << "Error accessing directory: " << e.what() << "\n";
    }

    int choice;
    cout << "Select file: ";
    cin >> choice;

    if (choice < 0 || choice >= (int)files.size())
    {
        cerr << "Invalid choice.\n";
        return;
    }
    file_name = files[choice].path().string();

    ifstream file(test_dir / file_name);
    string str;
    while (getline(file, str))
    {
        selected_file += str;
    }
}

int main()
{

    int choice;

    do
    {
        std::cout << "\n=============================\n";
        std::cout << "         MAIN MENU\n";
        std::cout << "=============================\n";
        std::cout << "Current Selected file: " << file_name << endl;
        std::cout << "Using file: " << (using_file ? "True" : "False") << endl;
        std::cout << "1. Regex to NFA (Exact Matching)\n";
        std::cout << "2. Approximate Matching\n";
        std::cout << "3. XML Validation using PDA\n";
        std::cout << "4. RNA secondary using PDA\n";
        std::cout << "5. Load File\n";
        std::cout << "6. Use file?\n";
        std::cout << "7. Exit\n";
        std::cout << "Your choice: ";

        if (!(std::cin >> choice))
        {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            choice = 0;
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
            runRegexExact();
            break;
        case 2:
            runApproximate();
            break;
        case 3:
            runXMLValidation();
            break;
        case 4:
            validateRNA();
            break;
        case 5:
            loadfile();
            break;
        case 6:
            using_file = !using_file;
            break;
        case 7:
            std::cout << "Exiting program. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Please select a number from 1 to 5.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}
