#ifndef XMLVALIDATE_H
#define XMLVALIDATE_H

#include <string>
#include <vector>

using namespace std;

// Tokenizer: converts <tag> -> "tag", </tag> -> "/tag"
vector<string> tokenizeXML(const string& xml);

// PDA-based XML validator with stack simulation output
bool validateXML(const string& xml, bool debug = true);

#endif