#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include "generatePre.hpp"
#include "trimfunc.hpp"
#include "allCaps.hpp"
using namespace std;

void generatePre(string filename){
    fstream infile, outfile, preEQU, preMACRO, preExpand;
    string line, aux, ext = ".asm";
    int len = 0, st_flag = 0, sd_flag = 0, if_flag = 1, exp_flag = 0, size = 0;
    size_t found;
    map<string, string> labelsMap;
    map<string, int> macroBody;
    map<string, int> macroArgsCount;
    map<string, string> macroArgs;

    infile.open(filename, std::fstream::in | std::fstream::out);

    if(!infile){
        cerr << "File not found." << endl;
        return;
    }

    found = filename.rfind(ext);
    filename.replace(found, ext.length(), ".pre");

    preEQU.open("preEQU.txt", fstream::in | fstream::out | fstream::trunc);
    outfile.open(filename, fstream::in | fstream::out | fstream::trunc);

    while(getline(infile, line)){ //remove unnecessary
        found = line.find_first_not_of(" \f\n\r\t\v");
        if(found != string::npos){
            line = trimAll(line);
            line.erase(remove(line.begin(), line.end(), '\t'), line.end());
            line = removeInBetween(line);
            line = removeComment(line);
            line = allCaps(line);
            if(line.size() > 0)
                preEQU << line << endl;
        }
    }

    infile.close();
    preEQU.clear();
    preEQU.seekg(0);
    // preMACRO.open("preMACRO.txt", fstream::in | fstream::out | fstream::trunc);

    while(getline(preEQU, line)){//steps preceding EQU
        if(line == "SECTION TEXT")
            st_flag = 1;
        if(!st_flag){
            if((found = line.find(':')) != string::npos){
                treatEQU(line, preEQU, labelsMap);
            }
        }
        else if(st_flag && (line != "SECTION TEXT") && (line != "SECTION DATA")){
            line = subsLabel(line, labelsMap);
            if(isIF(line))
                if_flag = evalIF(line, preEQU);
            if(if_flag){
                outfile << line << endl;
            }
            else if(!if_flag)
                if_flag = 1;
        }
    }

    // preMACRO.clear();
    // preMACRO.seekg(0);
    // preExpand.open("preExpand.txt", fstream::in | fstream::out | fstream::trunc);

    // while(getline(preMACRO, line)){//steps preceding MACRO
    //     if((found = line.find(':')) != string::npos)
    //         line = treatMACRO(line, preMACRO, macroBody, macroArgsCount, macroArgs);
    //     if(line != "ENDMACRO"){
    //         preExpand << line << endl;
    //     }
    // }

    // preMACRO.clear();
    // preMACRO.seekg(0);
    // preExpand.clear();
    // preExpand.seekg(0);

    // while(getline(preExpand, line)){//expand MACRO
    //     exp_flag = 0;
    //     if(line == "SECTION DATA")
    //         sd_flag = 1;
    //     if(!sd_flag)
    //         exp_flag = expandMacro(line, macroBody, macroArgsCount, macroArgs, preMACRO, outfile);
    //     if(!exp_flag){
    //         if(preExpand.peek() == EOF)
    //             outfile << line;
    //         else
    //             outfile << line << endl;
    //     }
    // }

    outfile.close();
    preEQU.close();
    // preMACRO.close();
    // preExpand.close();

    // macroBody.clear();
    // macroArgs.clear();
    // macroArgsCount.clear();

    remove("preEQU.txt");
    // remove("preMACRO.txt");
    // remove("preExpand.txt");
}

void treatEQU(string line, fstream& file, map<string, string>& labelsMap){ //Check EQU on current or next line and associate value to label with map
    size_t pos;
    string label, value, equ = " EQU ";

    if((pos = line.find(':')) != string::npos)
        label = line.substr(0, pos);

    if((pos = line.find(equ)) != string::npos)
        value = line.substr(pos + equ.length(), line.length() - pos);

    else{
        getline(file, line);
        value = line.substr(equ.length() - 1, line.length() - (equ.length() - 1));
    }

    labelsMap[label] = value;
}

// string treatMACRO(string line, fstream& file, map<string, int>& macroBody, map<string, int>& macroArgsCount, map<string, string>& macroArgs){ //Check MACRO on current or next line and store info into maps
//     size_t pos;
//     string label, params, keyword = "MACRO", og_line;
//     int n_args = 0, offset = 2, checkpoint = 0, space_flag = 1;

//     og_line = line;
//     if((pos = line.find(':')) != string::npos)
//         label = line.substr(0, pos);

//     pos = line.find(" "+keyword);//find " MACRO"
//     if(pos == string::npos){
//         offset--;
//         checkpoint = file.tellg();
//         getline(file, line);
//         pos = line.find(keyword+" ");//next line then find "MACRO "
//     }

//     if(pos == string::npos){
//         pos = line.find(keyword);//find "MACRO"
//         space_flag = 0;
//     }

//     if(pos == string::npos){
//         file.seekg(checkpoint);
//         return og_line;
//     }

//     n_args = count(line.begin() + (pos+keyword.length()+space_flag), line.end(), '&');
//     macroArgsCount[label] = n_args;
//     if(n_args){
//         params = line.substr(pos+keyword.length()+offset, line.length() - (pos+keyword.length()+offset));
//         params.erase(remove(params.begin(), params.end(), ','), params.end());
//         params.erase(remove(params.begin(), params.end(), ' '), params.end());
//         macroArgs[label] = params;
//     }
//     macroBody[label] = file.tellg();
//     while(line != "ENDMACRO")
//         getline(file, line);

//     return line;
// }

//swap macro call with its body and respective parameters
// int expandMacro(string line, map<string, int>& macroBody, map<string, int>& macroArgsCount, map<string, string>& macroArgs, fstream& source, fstream& output){
//     string aux, args, input_line, params = " ", params_aux, sub_arg, label = "";
//     map<string, int>::iterator bodyKey;
//     map<string, string> arg_list;
//     int exp_flag = 0, n_args = 0, wp = 0, ecom = 0, line_args = 0;
//     size_t pos = 0;

//     input_line = line;
//     aux = input_line.substr(0, input_line.find_first_of(" \f\n\r\t\v"));
//     if((bodyKey = macroBody.find(aux)) != macroBody.end()){
//         exp_flag = 1;
//         source.seekg(macroBody[aux]);
//         n_args = macroArgsCount[aux];
//         getline(source, line);

//         while(line != "ENDMACRO"){
//             if((pos = line.find(":")) != string::npos){
//                 label = line.substr(0, pos+2);
//                 line.erase(0, pos+2);
//             }
//             else label = "";

//             line_args = count(line.begin(), line.end(), '&');

//             if(n_args){
//                 if(line_args){
//                     args = macroArgs[aux];
//                     if(n_args == 1){
//                         if((pos = line.find(" "+args)) != string::npos){
//                             params = input_line.substr(input_line.find_first_of(" ")+1, input_line.length()-(input_line.find_first_of(" ")+1));
//                             line.replace(pos+1, args.length(), params);
//                         }
//                     }
//                     else if(n_args == 2){//unefficient and lazy way of replacing macro parameters by its respective inputs, but it works...
//                         sub_arg = args.substr(0, args.rfind("&"));
//                         params = input_line.substr(input_line.find_first_of(" ")+1, input_line.length()-(input_line.find_first_of(" ")+1));
//                         params.erase(params.find_first_of(","), 1);
//                         arg_list[sub_arg]=params.substr(0, params.find_first_of(" "));

//                         args.erase(0, args.rfind("&"));
//                         params.erase(0, params.find_first_of(" ")+1);
//                         arg_list[args]=params.substr(0, params.find_first_of(" "));
                        
//                         wp = count(line.begin(), line.end(), ' ');
//                         if(wp == 1){
//                             params_aux = line.substr(line.find_first_of(" ")+1, line.length() - (line.find_first_of(" ")+1));
//                             line.replace(line.find_first_of(" ")+1, params_aux.length(), arg_list[params_aux]);
//                         }
//                         if(wp == 2){
//                             ecom = count(line.begin(), line.end(), '&');
//                             if(ecom == 1){
//                                 params_aux = line.substr(0, line.find_first_of(","));
//                                 if((pos = line.find("&"))!=string::npos){
//                                     params_aux = line.substr(line.find_first_of("&"), line.find_first_of(",")-(line.find_first_of("&")));
//                                     line.replace(line.find_first_of("&"), params_aux.length(), arg_list[params_aux]);
//                                 }
//                                 else{
//                                     params_aux = line.substr(line.find_first_of("&"), line.length()-(line.find_first_of("&")));
//                                     line.replace(line.find_first_of("&"), params_aux.length(), arg_list[params_aux]);
//                                 }
//                             }
//                             else if(ecom == 2){
//                                 params_aux = line.substr(line.find_first_of(" ")+1, line.find_first_of(",")-(line.find_first_of(" ")+1));
//                                 line.replace(line.find_first_of(" ")+1, params_aux.length(), arg_list[params_aux]);

//                                 params_aux = line.substr(line.find_first_of(",")+2, line.length()-(line.find_first_of(",")+2));
//                                 line.replace(line.find_first_of(",")+2, params_aux.length(), arg_list[params_aux]);
//                             }
//                         }
//                     }
//                 }
//             }
//             output << label << line << endl;
//             getline(source, line);
//         }
//     }

//     source.clear();
//     source.seekg(0);
//     arg_list.clear();

//     return exp_flag;
// }

string subsLabel(string line, map<string, string>& labelsMap){ //Replace occurrence of declared lables
    string output, ws = " ", token;
    size_t pos = 0, pos_r = 0;
    map<string, string>::iterator key;

    output = line;

    do{
        pos = line.find(ws);
        token = line.substr(0, pos);
        if((key = labelsMap.find(token)) != labelsMap.end())
            output.replace(output.begin() + pos_r, output.begin() + pos, labelsMap[token]);

        line.erase(0, pos + ws.length());
        pos_r = pos_r + pos + ws.length();
    }while(pos != string::npos);

    return output;
}

bool isIF(string line){ //Check if line contains IF directive
    string ws = " ", token, value;
    size_t pos = 0;

    do{
        pos = line.find(ws);
        token = line.substr(0, pos);
        if(token == "IF"){
            return true;
        }
        line.erase(0, pos + ws.length());
    }while(pos != string::npos);

    return false;
}

int evalIF(string& line, fstream& file){ //Evaluate IF statement and prevent line that contains IF directive from passing to output file
    string ws = " ", token;
    size_t pos = 0;
    int value = 0;

    while((pos = line.find(ws)) != string::npos){
        token = line.substr(0, pos);
        line.erase(0, pos + ws.length());
    }

    value = stoi(line);
    getline(file, line);//skip current line

    return value;
}