#include <iostream>
#include <string>
#include <map>
using namespace std;

string generatePre(string);

void treatEQU(string, fstream&, map<string, string>&);

string subsLabel(string, map<string, string>&);

bool isIF(string);

int evalIF(string&, fstream&);

// string treatMACRO(string, fstream&, map<string, int>&, map<string, int>&, map<string, string>&);

// int expandMacro(string, map<string, int>&, map<string, int>&, map<string, string>&, fstream&, fstream&);