#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string allCaps(string str){//capitalize line
    string caps = str;

    for_each(caps.begin(), caps.end(), [](char &c){
        c = ::toupper(c);
    });

    return caps;
}