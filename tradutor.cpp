#include <iostream>
#include <fstream>
#include "generatePre.hpp"
using namespace std;

int main (int argc, char **argv) {
  string filename;

  filename = argv[1];
  generatePre(filename);
    
  return 0;
}