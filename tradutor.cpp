#include <iostream>
#include <fstream>
#include "generatePre.hpp"
#include "translate.hpp"
using namespace std;

int main (int argc, char **argv) {
  string filename, preTranslate;

  filename = argv[1];
  preTranslate = generatePre(filename);
  translate(preTranslate);
    
  return 0;
}