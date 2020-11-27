#include <iostream>
#include <string>
#include "trimfunc.hpp"
using namespace std;

string trimRight(const string& s, string delimiters){//remove unnecessary from the right
  return s.substr(0, s.find_last_not_of(delimiters) + 1);
}

string trimLeft(const string& s, string delimiters){//remove unnecessary from the left
  return s.substr(s.find_first_not_of(delimiters));
}

string trimAll(const string& s, string delimiters){//remove unnecessary from both sides
  return trimLeft(trimRight(s, delimiters), delimiters);
}

string removeInBetween(const string& s){//remove unnecessary white spaces in between valid tokens
  string out;
  int flag = 0, len = 0, i = 0;

  len = s.length();
  for(i = 0; i < len; i++){
    if(s[i] != ' '){
      out.append(1, s[i]);
      flag = 0;
    }
    else if(s[i] == ' ' && flag == 0){
      out.append(1, s[i]);
      flag = 1;
    }
  }

  return out;
}

string removeComment(const string& s){//remove everything from the line after a ";"
  string out;
  int len = 0, i = 0;
  size_t pos;

  pos = s.find_first_of(';');
  out = s.substr(0, pos);
  len = out.length();

  if(out[len-1] == ' ')
    out.erase(out.end()-1);

  return out;
}