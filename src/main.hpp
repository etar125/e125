#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "extension.hpp"
#include "color.hpp"
#include "str.hpp"
#define _clearmem fill(mem.begin(), mem.end(), 0);

using namespace std;
namespace fs = filesystem;

string ver = "e125 v2.24.7_7a";
string lang[10]={"MISSING"};

vector<ext> elist;
short current = 0;
int mems = 256;
vector<char> mem;

void einit();
int init() {
    cout << MAGENTA << ver << RESET << endl;
    cout << YELLOW << "..." << RESET << endl;

    ifstream lcl("lang/cfg");
    string temp = "";
    if(!lcl) temp = "en";
    else getline(lcl, temp);
    temp = "lang/"+temp;

    lcl = ifstream(temp, ios::binary);
    if(!lcl) return 1;
    char c = 0;
    while(getline(lcl, temp)) {
        lang[c]=temp;
        ++c;
    }

    cout << GREEN << lang[0] << RESET << endl;

    einit();
    mem.resize(mems);
    return 0;
}

void einit() {
    cout << YELLOW << lang[1] << RESET << endl;
    string path = "exts";
    fs::create_directory(path);
    ifstream x;
    string temp;
    string name = "";
    string desc = "";
    vector<string> code;
    char a = 0;
    char c = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        x = ifstream(entry.path());
        while(getline(x, temp)) {
            if(a == 0) {
                name = temp;
                cout << BLUE << lang[2] << " " << name << RESET << endl;
                a++;
            } else if(a == 1) {
                desc = temp;
                a++;
            } else {
                code.push_back(temp);
            }
        }
        a = 0;
        elist.push_back(ext(name, desc, code));
        code.clear();
        ++c;
    }
    cout << GREEN << lang[3] << " " << to_string(c) << RESET << endl;
}

void loop() {
    string str;
    vector<string> a;
    while(true) {
        cout << "# ";
        getline(cin, str);
        a = strsp(str, ' ');
        if(a[0] == "list") {
            cout << BLUE;
            for(ext e : elist) cout << e.name << "   " << e.desc << endl;
            cout << RESET;
        }
        else if(a[0] == "ch") {
            bool f = false;
            for(short i = 0; i < elist.size(); i++) {
                if(elist[i].name == a[1]) {
                    current = i;
                    f = true;
                    break;
                }
            }
            if(!f) cout << YELLOW << lang[4] << RESET << endl;
        }
        else if(a[0] == "cl") {_clearmem; }
        else if(a[0] == "set") {
            int pos = stoi(a[1]);
            int val = stoi(a[2]);
            if(pos < mems) mem[pos] = (char)val;
            else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "print") {
            int pos = stoi(a[1]);
            int len = stoi(a[2]);
            if(pos + len <= mems) {
                for(int i = 0; i < len - 1; i++) cout << to_string((int)(mem[pos + i])) << " ";
                cout << to_string((int)(mem[pos])) << endl;
            } else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "res") {
            int size = stoi(a[1]);
            mem.resize(size);
            mems = size;
        } else if(a[0] == "run") {
            int pos = stoi(a[1]);

            if(pos < mems) {
                elist[current].ss.set("pos", to_string(pos));
                TSSException te = elist[current].ss.docode(elist[current].code);
                if(te.index != -1) {
                    cout << RED << "ext: " << elist[current].name << endl;
                    cout << "index " << to_string(te.index) << " token:[ ";
                    if(te.token.type == tkntp::com) cout << "command, ";
                    else if(te.token.type == tkntp::var) cout << "variable, ";
                    else if(te.token.type == tkntp::lab) cout << "label, ";
                    else if(te.token.type == tkntp::val) cout << "value, ";
                    cout << "\"" << te.token.val << "\" ]\n" << te.message << RESET << endl;
                }
            } else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "cur") {
            cout << BLUE << elist[current].name << "   " << elist[current].desc << RESET << endl;
        } else if(a[0] == "exit") break;
    }
}

void tss::gfunc(string name) {

}

/* Будет переписано
 * mem.resize(size);
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





































