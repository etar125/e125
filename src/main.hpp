#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "extension.hpp"

using namespace std;

string GREEN = "\033[32m";
string RED = "\033[31m";
string YELLOW = "\033[33m";
string LBLUE = "\033[94m";
string RESET = "\033[0m";

string ver = "e125 v2.24.7_6 TEST";

string lang[10]={"MISSING"};

vector<ext> elist;

int init()
{
    cout << ver << endl;
    cout << YELLOW << "..." << RESET << endl;
    ifstream lcl("lang/cfg");
    string temp = "";
    if(!lcl) temp = "en";
    else getline(lcl, temp);
    temp = "lang/"+temp;
    ifstream lng(temp, ios::binary);
    if(!lng) return 1;
    char c = 0;
    while(getline(lng, temp))
    {
        lang[c]=temp;
        ++c;
    }
    cout << GREEN << lang[0] << RESET << endl;
    return 0;
}

int einit()
{

    return 0;
}

void tss::gfunc(string name)
{

}

/* Будет переписано
void tss::gfunc(std::string name)
{
    if(name == "getb") // get byte: $position name
        tss::set(tss::stack[1], std::to_string(list[current].GetFromMemory(sx(std::stoi(tss::stack[0])))));
    else if(name == "setb") // set byte: $position $value
        list[current].LoadToMemory(bx(std::stoi(tss::stack[1])), sx(std::stoi(tss::stack[0])));
    else if(name == "getw") // get word: like getb
    {
        byte f = list[current].GetFromMemory(sx(std::stoi(tss::stack[0])));
        byte s = list[current].GetFromMemory(sx(std::stoi(tss::stack[0]) + 1));
        word w = f + (s << 8);
        tss::set(tss::stack[2], std::to_string(w));
    }
    else if(name == "setw") // set word: like setb
    {
        word w = sx(std::stoi(tss::stack[1]));
        word p = sx(std::stoi(tss::stack[0]));
        byte f = w >> 8;
        byte s = (w << 8) >> 8;
        list[current].LoadToMemory(f, p);
        list[current].LoadToMemory(s, p + 1);
    }
    else if(name == "resmem") // resize memory
        list[current].SetMemSize(std::stoi(tss::stack[0]));
    else if(name == "putstr")
    {
        std::string str = "";
        bool spec = false;
        for(char d : tss::stack[0])
        {
            if(d == '\\' && !spec) spec = true;
            else if (d == '\\' && spec) spec = false;
            else if(spec)
            {
                if(d == 'n') str += '\n';
                else if(d == 't') str += '\t';
                else if(d == 'a') str += '\033';
                spec = false;
            }
            else str += d;
        }
        std::cout << str;
    }
    else if(name == "putch")
    {
        std::cout << std::stoi(tss::stack[0]);
    }
    else if(name == "getline")
    {
        std::string str;
        getline(std::cin, str);
        tss::set(tss::stack[0], str);
    }
} */





































