#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include "translate.hpp"
using namespace std;

void translate(string filename){
    fstream infile, outfile, dataToFile, funcToFile;
    string ext = ".pre", line, aux = "", aux2 = "", rot, param = "";
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

    dataToFile.open("dataToFile.txt", std::fstream::in | std::fstream::out);
    while(getline(dataToFile, line))
        outfile << line << endl;
    dataToFile.close();

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
                
                if((found = line.find(" SPACE")) != string::npos){
                    data_flag = 0;
                    if(!bss_flag){
                        bss_flag = 1;
                        outfile << "section .bss" << endl;
                    }
                    if((found = line.find("SPACE ")) != string::npos)
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
        if((found = line.find(":")) != string::npos){
            if(line.length() == found + 1)
                outfile << line << endl;
            else{
                outfile << line.substr(0, found + 1) << endl;
                aux = line.substr(line.find(" ") + 1, line.length());
                aux2 = aux.substr(0, aux.find(" "));
                param = aux.substr(aux.find(" ") + 1, aux.length());
                translate_inst(aux2, inst_code, outfile, param);
            }
        }
        else{
            aux = line.substr(0, line.find(" "));
            param = line.substr(line.find(" ") + 1, line.length());
            translate_inst(aux, inst_code, outfile, param);
        }
    }while(line != "SECTION DATA");

    funcToFile.open("funcToFile.txt", std::fstream::in | std::fstream::out);
    while(getline(funcToFile, line))
        outfile << line << endl;
    funcToFile.close();

    infile.close();
    outfile.close();
}

void translate_inst(string inst, map<string, int>& inst_code, fstream& outfile, string param){
    size_t found, n;
    string num, aux = "";
    int val;

    if((found = param.find(" + ")) != string::npos){
        n = count(param.begin(), param.end(), '+');
        if(n == 1){
            num = param.substr(param.find("+") + 2, param.find(",") - (param.find("+") + 2));
            val = stoi(num) * 4;
            param.replace(param.find("+") + 2, num.length(), to_string(val));
        }
        else if(n == 2){
            aux = param.substr(0, param.find(","));
            num = aux.substr(aux.find("+") + 2, aux.find(",") - (aux.find("+") + 2));
            val = stoi(num) * 4;
            aux.replace(aux.find("+") + 2, num.length(), to_string(val));
            aux = "[" + aux + "]";

            param = param.substr(param.find(",") + 2, param.length());
            num = param.substr(param.find("+") + 2, param.find(",") - (param.find("+") + 2));
            val = stoi(num) * 4;
            param.replace(param.find("+") + 2, num.length(), to_string(val));
            param = "[" + param + "]";
        }
    }

    switch(inst_code[inst]){
        case 1: 
            outfile << "add eax, DWORD [" << param << "] ; ADD" << endl;
            break;
        case 2: 
            outfile << "sub eax, DWORD [" << param << "] ; SUB" << endl;
            break;
        case 4:
            outfile << "cdq ;DIV" << endl;
            outfile << "mov ebx, DWORD [" << param << "]" << endl;
            outfile << "idiv ebx" << endl;
            break;
        case 5:
            outfile << "jmp " << param << " ; JMP" << endl;
            break;
        case 6:
            outfile << "cmp eax, 0 ; JMPN" << endl;
            outfile << "jl " << param << endl;
            break;
        case 7:
            outfile << "cmp eax, 0 ; JMPP" << endl;
            outfile << "jg " << param << endl;
            break;
        case 8:
            outfile << "cmp eax, 0 ; JMPZ" << endl;
            outfile << "je " << param << endl;
            break;
        case 9:
            if(found == string::npos){
                outfile << "mov ebx, DWORD [" << param.substr(0, param.find(",")) << "] ; COPY" << endl;
                outfile << "mov [" << param.substr(param.find(",") + 2, param.length()) << "], ebx" << endl;
            }
            else{
                if(n == 1){
                    outfile << "mov ebx, DWORD [" << param.substr(0, param.find(",")) << "] ; COPY" << endl;
                    outfile << "mov [" << param.substr(param.find(",") + 2, param.length()) << "], ebx" << endl;
                }
                else if(n == 2){
                    outfile << "mov ebx, DWORD " << aux << " ; COPY" << endl;
                    outfile << "mov " << param << ", ebx" << endl;
                }
            }
            break;
        case 10:
            outfile << "mov eax, DWORD [" << param << "] ; LOAD" << endl;
            break;
        case 11:
            outfile << "mov [" << param << "], eax ; STORE" << endl;
            break;
        case 12:
            outfile << "push eax ; INPUT" << endl;
            outfile << "push " << param << endl;
            outfile << "call LeerInteiro" << endl;
            outfile << "mov [_result], eax" << endl;
            outfile << "call _displayNchars" << endl;
            outfile << "pop eax" << endl;
            break;
        case 13:
            outfile << "push eax ; OUTPUT" << endl;
            outfile << "push DWORD " << "[" << param << "]" << endl;
            outfile << "call EscreverInteiro" << endl;
            outfile << "pop eax" << endl;
            break;
        case 14:
            outfile << "push eax ; C_INPUT" << endl;
            outfile << "push " << param << endl;
            outfile << "call LeerChar" << endl;
            outfile << "mov [_result], eax" << endl;
            outfile << "call _displayNchars" << endl;
            outfile << "pop eax" << endl;
            break;
        case 15:
            outfile << "push eax ; C_OUTPUT" << endl;
            outfile << "push DWORD " << "[" << param << "]" << endl;
            outfile << "call EscreverChar" << endl;
            outfile << "pop eax" << endl;
            break;
        case 16:
            outfile << "push eax ; S_INPUT" << endl;
            outfile << "push " << param.substr(0, param.find(",")) << endl;
            outfile << "push DWORD " << param.substr(param.find(",") + 2, param.length()) << endl;
            outfile << "call LeerString" << endl;
            outfile << "mov [_result], eax" << endl;
            outfile << "call _displayNchars" << endl;
            outfile << "pop eax" << endl;
            break;
        case 17:
            outfile << "push eax ; S_OUTPUT" << endl;
            outfile << "push " << param.substr(0, param.find(",")) << endl;
            outfile << "push DWORD " << param.substr(param.find(",") + 2, param.length()) << endl;
            outfile << "call EscreverString" << endl;
            outfile << "pop eax" << endl;
            break;
        case 18:
            outfile << "mov eax, 1 ; STOP" << endl;
            outfile << "mov ebx, 0" << endl;
            outfile << "int 80h" << endl;
            break;
        default:
            outfile << "";
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