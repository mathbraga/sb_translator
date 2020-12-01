#include <iostream>
#include <string>
#include <map>
using namespace std;

void translate(string);
void translate_inst(string, map<string, int>&, fstream&, string);
void init_inst(map<string, int>&);