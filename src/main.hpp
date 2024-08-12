#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "extension.hpp"
#include "str.hpp"
#define _clearmem fill(mem.begin(), mem.end(), 0);
#define ecur elist[current]
#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int

using namespace std;
namespace fs = filesystem;

string ver = "e125 v3.24.8_24b";
string lang[10]={"MISSING"};
bool debug = false;
bool test = false;

struct vmem {
    vector<uchar> mem;

    void resize(uint size) {
        mem.resize(size / 8);
    } void clear() { fill(mem.begin(), mem.end(), 0); }

    void set(uint pos, bool val) {
        if(pos / 8 < mem.size()) {
            if(val) mem[pos / 8] |= val << (pos % 8);
            else mem[pos / 8] &= ~(val << (pos % 8));
        }
        else cout << RED << lang[6] << RESET << endl;
    } bool get(uint pos) {
        if(pos / 8 < mem.size()) return (bool((1 << (pos % 8)) & mem[pos / 8]));
        else cout << RED << lang[6] << RESET << endl;
        return false;
    } vector<bool> get_range(uint pos, uint range) {
        if((pos + range) / 8 < mem.size()) {
            vector<bool> r;
            while(range != 0) {
                r.push_back(get(pos));
                ++pos;--range;
            } return r;
        } else cout << RED << lang[6] << RESET << endl;
        return {};
    } void set_range(uint pos, vector<bool> range) {
        if((pos + range.size()) / 8 < mem.size()) {
            for(bool a : range) {
                set(pos, a); ++pos;
            }
        } else cout << RED << lang[6] << RESET << endl;
    }

    vmem() { resize(256); }
    vmem(uint size) { resize(size); }
};

vector<ext> elist;
short current = 0;
int mems = 256;
vector<uchar> mem;

void einit();
int init() {
    cout << MAGENTA << ver;
    if(test) cout << " TEST";
    if(debug) cout << " DEBUG";
    cout << RESET << endl << YELLOW << "..." << RESET << endl;

    ifstream lcl("lang/cfg");
    string temp = "";
    if(!lcl) temp = "en";
    else getline(lcl, temp);
    temp = "lang/"+temp;

    lcl = ifstream(temp, ios::binary);
    if(!lcl) return 1;
    uchar c = 0;
    while(getline(lcl, temp)) {
        lang[c]=temp;
        ++c;
    }

    cout << GREEN << lang[0] << RESET << endl;

    if(test) {
        vmem a;
        a.set(0, true);
        a.set(16, true);
        a.set(16, false);
        a.set(17, true);
        a.set(256, true);
        cout << a.get(0) << a.get(1) << a.get(17) << a.get(256) << endl;

        a.set_range(1, {true, false, true, false});
        for(bool a : a.get_range(1, 4)) cout << a;
        cout << endl;

        return 2;
    }

    einit();
    mem.resize(mems);
    _clearmem;
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
    uchar a = 0;
    uchar c = 0;
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
void savem(string name) {
    if(fs::exists(name)) remove(name.c_str());
    ofstream file(name);
    for(uchar a : mem) { file << a; }
    file.close();
} void loadm(string name) {
    ifstream file(name);
    string temp;
    getline(file, temp);
    mem.clear();
    for(uchar a : temp) mem.push_back(a);
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
            for(ushort i = 0; i < elist.size(); i++) {
                if(elist[i].name == a[1]) {
                    current = i;
                    f = true;
                    ecur.code.insert(ecur.code.begin(), "exit");
                    ecur.code.insert(ecur.code.begin(), "call init");
                    TSSException te = ecur.ss.docode(ecur.code);
                    if(te.index != -1) {
                        cout << RED << "ext: " << ecur.name << endl;
                        cout << "index " << to_string(te.index) << " token:[ ";
                        if(te.token.type == tkntp::com) cout << "command, ";
                        else if(te.token.type == tkntp::var) cout << "variable, ";
                        else if(te.token.type == tkntp::lab) cout << "label, ";
                        else if(te.token.type == tkntp::val) cout << "value, ";
                        cout << "\"" << te.token.val << "\" ]\n" << te.message << RESET << endl;
                    }
                    ecur.code.erase(ecur.code.begin(), ecur.code.begin() + 2);
                    break;
                }
            }
            if(!f) cout << YELLOW << lang[4] << RESET << endl;
        } else if(a[0] == "cl") {_clearmem; }
        else if(a[0] == "set") {
            int pos = stoi(a[1]);
            int val = stoi(a[2]);
            if(pos < mems) mem[pos] = (uchar)val;
            else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "print") {
            int pos = stoi(a[1]);
            int len = stoi(a[2]);
            if(pos + len <= mems) {
                for(int i = 0; i < len - 1; i++) cout << to_string((ushort)(mem[pos + i])) << " ";
                cout << to_string((ushort)(mem[pos])) << endl;
            } else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "res") {
            int size = stoi(a[1]);
            mem.resize(size);
            mems = size;
        } else if(a[0] == "run") {
            int pos = stoi(a[1]);

            if(pos < mems) {
                ecur.ss.set("pos", to_string(pos));
                TSSException te = ecur.ss.docode(ecur.code);
                if(te.index != -1) {
                    cout << RED << "ext: " << ecur.name << endl;
                    cout << "index " << to_string(te.index) << " token:[ ";
                    if(te.token.type == tkntp::com) cout << "command, ";
                    else if(te.token.type == tkntp::var) cout << "variable, ";
                    else if(te.token.type == tkntp::lab) cout << "label, ";
                    else if(te.token.type == tkntp::val) cout << "value, ";
                    cout << "\"" << te.token.val << "\" ]\n" << te.message << RESET << endl;
                }
            } else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "cur") {
            cout << BLUE << ecur.name << "   " << ecur.desc << RESET << endl;
        } else if(a[0] == "exit") { break; }
        else if(a[0] == "call") {
            ecur.code.insert(ecur.code.begin(), "exit");
            ecur.code.insert(ecur.code.begin(), "call " + a[1]);
            TSSException te = ecur.ss.docode(ecur.code);
            if(te.index != -1) {
                cout << RED << "ext: " << ecur.name << endl;
                cout << "index " << to_string(te.index) << " token:[ ";
                if(te.token.type == tkntp::com) cout << "command, ";
                else if(te.token.type == tkntp::var) cout << "variable, ";
                else if(te.token.type == tkntp::lab) cout << "label, ";
                else if(te.token.type == tkntp::val) cout << "value, ";
                cout << "\"" << te.token.val << "\" ]\n" << te.message << RESET << endl;
            }
            ecur.code.erase(ecur.code.begin(), ecur.code.begin() + 2);
        } else if(a[0] == "cls" || a[0] == "clear") { cout << "\033[H\033[J"; }
        else if (a[0] == "ver") { cout << MAGENTA << ver << RESET << endl; }
        else if (a[0] == "save") { savem(a[1]); }
        else if (a[0] == "load") { loadm(a[1]); }
        else if(a[0] == "help") {
            cout << "list\nch name\ncl\nset pos val\nprint pos len\nres size\nrun pos\ncur\ncall name\ncls\nclear\nver\nsave filename\nload filename\nexit" << endl;
        }
    }
}

void tss::gfunc(string name) {
    if(name == "getb") { // get byte: $pos var
        int pos = stoi(tss::stack[0]);
        if(pos < mems) {
            tss::set(
                tss::stack[1],
                to_string(mem[pos])
            );
        } else cout << RED << "[" << ecur.name << ", getb] " << lang[5] << ": $pos" << RESET << endl;
    } else if(name == "setb") { // set byte: $pos $num
        int pos = stoi(tss::stack[0]);
        if(pos < mems) {
            mem[pos] = (char)(stoi(tss::stack[1]));
        } else cout << RED << "[" << ecur.name << ", getb] " << lang[5] << ": $pos" << RESET << endl;
    } else if(name == "resmem") { // resize memory: $val
        int size = stoi(tss::stack[0]);
        mem.resize(size);
        mems = size;
    } else if(name == "putstr") { // put string: $val
        string str = "";
        bool spec = false;
        for(char d : tss::stack[0]) {
            if(d == '\\' && !spec) spec = true;
            else if (d == '\\' && spec) spec = false;
            else if(spec) {
                if(d == 'n') str += '\n';
                else if(d == 't') str += '\t';
                else if(d == 'a') str += '\033';
                spec = false;
            }
            else str += d;
        }
        cout << str;
    } else if(name == "putch") { // put char: $num
        cout << stoi(tss::stack[0]);
    } else if(name == "getline") { // get line: var
        string str;
        getline(cin, str);
        tss::set(tss::stack[0], str);
    } else if(name == "save") { savem(tss::stack[0]); }
    else if(name == "load") { loadm(tss::stack[0]); }
}
