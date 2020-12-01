#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "translate.hpp"
using namespace std;

void translate(string filename){
    fstream infile, outfile;
    string ext = ".pre", line, aux, rot;
    size_t found;
    int sd_flag = 0, bss_flag = 0, data_flag = 0;
    map<string, int> inst_code;

    init_inst(inst_code);

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
            outfile << "_start:" << endl;
        }
    }

    infile.clear();
    infile.seekg(0);

    do{
        getline(infile, line);
        if()
    }while(line != "SECTION DATA");

    infile.close();
    outfile.close();
}

void translate_inst(string inst, map<string, int>& inst_code, fstream& outfile, string param){
    switch(inst_code[inst]){
        case 12:
            outfile << "push eax" << endl;
            outfile << "push " << param << endl;
            outfile << "call LeerInteiro" << endl;
            outfile << "mov [_result], eax" << endl;
            outfile << "mov eax, 4" << endl;
            outfile << "mov ebx, 1" << endl;
            outfile << "mov ecx, _msg1" << endl;
            outfile << "mov edx, _msg1_s" << endl;
            outfile << "int 80h" << endl;
            outfile << "mov eax, 4" << endl;
            outfile << "mov ebx, 1" << endl;
            outfile << "mov ecx, _result" << endl;
            outfile << "mov edx, 4" << endl;
            outfile << "int 80h" << endl;
            outfile << "mov eax, 4" << endl;
            outfile << "mov ebx, 1" << endl;
            outfile << "mov ecx, _msg2" << endl;
            outfile << "mov edx, _msg2_s" << endl;
            outfile << "int 80h" << endl;
            outfile << "pop eax" << endl;
    }
}

void init_inst(map<string, int>& inst_code){
    inst_code["ADD"]=1;
    inst_code["SUB"]=2;
    inst_code["MULT"]=3;
    inst_code["DIV"]=4;
    inst_code["JMP"]=5;
    inst_code["JMPN"]=6;
    inst_code["JMPP"]=7;
    inst_code["JMPZ"]=8;
    inst_code["COPY"]=9;
    inst_code["LOAD"]=10;
    inst_code["STORE"]=11;
    inst_code["INPUT"]=12;
    inst_code["OUTPUT"]=13;
    inst_code["C_INPUT"]=14;
    inst_code["C_OUTPUT"]=15;
    inst_code["S_INPUT"]=16;
    inst_code["S_OUTPUT"]=17;
    inst_code["STOP"]=18;
}