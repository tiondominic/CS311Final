#ifndef XMLVALIDATE_H
#define XMLVALIDATE_H

#include <string>
#include <vector>

using namespace std;

vector<string> tokenizeXML(const string& xml);

bool validateXML(const string& xml, bool debug = true);

bool validateRNA(const string& structure);

#endif