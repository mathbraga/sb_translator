#include <iostream>
#include <string>
using namespace std;

string trimRight(const string&, string delimiters = " \f\n\r\t\v");

string trimLeft(const string&, string delimiters = " \f\n\r\t\v");

string trimAll(const string&, string delimiters = " \f\n\r\t\v");

string removeInBetween(const string&);

string removeComment(const string&);