#include <iostream>
#include <fstream>
#include <string>
#include "translate.hpp"
using namespace std;

void translate(string filename){
    fstream infile, outfile;
    string ext = ".pre", line, aux, rot;
    size_t found;
    int sd_flag = 0, bss_flag = 0, data_flag = 0;

    infile.open(filename, std::fstream::in | std::fstream::out);

    if(!infile){
        cerr << "File not found." << endl;
        exit(EXIT_FAILURE);
    }

    found = filename.rfind(ext);
    filename.replace(found, ext.length(), ".s");

    outfile.open(filename, fstream::in | fstream::out | fstream::trunc);
    outfile << "global _start" << endl;

    while(getline(infile, line)){
        if(line == "SECTION DATA")
            sd_flag = 1;
        if(sd_flag){
            if((found = line.find(":")) != string::npos){
                rot = line.substr(0 , found);
                if((found + 1) == (line.length()))
                    getline(infile, line);

                if((found = line.find("CONST ")) != string::npos){
                    bss_flag = 0;
                    if(!data_flag){
                        data_flag = 1;
                        outfile << "section .data" << endl;
                    }
                    aux = rot + " dd " + line.substr(line.rfind(" ") + 1, line.length());
                    outfile << aux << endl;
                }
                
                if((found = line.find("SPACE")) != string::npos){
                    data_flag = 0;
                    if(!bss_flag){
                        bss_flag = 1;
                        outfile << "section .bss" << endl;
                    }
                    if((found = line.find(" + ")) != string::npos)
                        aux = rot + " resd " + line.substr(line.rfind(" ") + 1, line.length());
                    else
                        aux = rot + " resd 1";
                    
                    outfile << aux << endl;
                }
            }
        }
        if(infile.peek() == EOF){
            outfile << "section .text" << endl;
            outfile << "_start:";
        }
    }

    infile.close();
    outfile.close();
}