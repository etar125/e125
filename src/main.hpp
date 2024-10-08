#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "extension.hpp"
#include "str.hpp"
#define ecur elist[current]
#define _clearmem fill(omem.begin(), omem.end(), 0);
#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ver "e125 v4.00_24914"

using namespace std;
namespace fs = filesystem;

string lang[23]={"MISSING"};
bool debug = false;
bool test = false;

struct vmem {
    vector<uchar> mem;

    void resize(uint size) {
        mem.resize(size / 8);
    } void clear() { fill(mem.begin(), mem.end(), 0); }

    void set(int pos, bool val) {
        if(pos / 8 < mem.size()) {
            if(val) mem[pos / 8] |= val << (pos % 8);
            else mem[pos / 8] &= ~(val << (pos % 8));
        }
        else cout << RED << lang[6] << RESET << endl;
    } bool get(int pos) {
        if(pos / 8 < mem.size()) return (bool((1 << (pos % 8)) & mem[pos / 8]));
        else cout << RED << lang[6] << RESET << endl;
        return false;
    } vector<bool> get_range(int pos, int range) {
        if((pos + range) / 8 < mem.size()) {
            vector<bool> r;
            while(range != 0) {
                r.push_back(get(pos));
                ++pos;--range;
            } return r;
        } else cout << RED << lang[6] << RESET << endl;
        return {};
    } void set_range(int pos, vector<bool> range) {
        if((pos + range.size()) / 8 < mem.size()) {
            for(bool a : range) {
                set(pos, a); ++pos;
            }
        } else cout << RED << lang[6] << RESET << endl;
    } size_t size() { return mem.size(); }

    vmem() { resize(256); }
    vmem(uint size) { resize(size); }
};

vector<ext> elist;
short current = 0;
int mems = 256;
vmem nmem;
vector<uchar> omem;

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

    einit();
    if(test) {
    	nmem.resize(mems);
    	nmem.clear();
    } else {
    	omem.resize(mems);
    	_clearmem;
    }
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
    if(!test) for(uchar a : omem) file << a;
    else { }
    file.close();
} void loadm(string name) {
    ifstream file(name);
    string temp;
    getline(file, temp);
    if(!test) { omem.clear();
    for(uchar a : temp) omem.push_back(a); }
    else { }
}

string btos(vector<bool> bits) {
	string h = "";
	if(bits.size() != 0) {
		for(bool a : bits) {
			if(a) h += "1";
			else h += "0";
		}
	} return h;
} vector<bool> stob(string str) {
	vector<bool> h;
	for(char a : str) {
		if(a == '1') h.push_back(true);
		else if(a == '0') h.push_back(false);
	} return h;
} int btoi(vector<bool> bits) {
	int h = 0;
	int s = bits.size() - 1;
	for(bool a : bits) {
		h += (int)a * (2^s);
		s--;
	} return h;
} vector<bool> itob(int num) {
	vector<bool> h;
	while(num != 0) {
		h.push_back(num & 2);
		num = num / 2;
	} return h;
}


void loop() {
    string str;
    vector<string> a;
    cout << lang[9] << endl;
    while(true) {
        cout << "# ";
        getline(cin, str);
        a = strsp(str, ' ');
        if(a[0] == "list") {
            cout << BLUE;
            for(ext e : elist) cout << e.name << "   " << e.desc << endl;
            cout << RESET;
        }
        else if(a[0] == "ch" && a.size() == 2) {
            bool f = false;
            for(ushort i = 0; i < elist.size(); i++) {
                if(elist[i].name == a[1]) {
                    current = i;
                    f = true;
                    ecur.code.insert(ecur.code.begin(), "exit");
                    ecur.code.insert(ecur.code.begin(), "call init");
                    TSSException te = ecur.ss.docode(ecur.code, debug);
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
        } else if(a[0] == "cl") { 
        	if(!test) { _clearmem; }
        	else { }
        }
        else if(a[0] == "set" && a.size() == 3) {
            int pos = stoi(a[1]);
            int val = stoi(a[2]);
            if(pos < mems) { 
            	if(!test) omem[pos] = (uchar)val;
            	else { }
            }
            else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "print" && a.size() == 3) {
            int pos = stoi(a[1]);
            int len = stoi(a[2]);
            if(pos + len <= mems) {
            	if(test) { for(bool a : nmem.get_range(pos, len)) cout << a;
            	cout << endl; }
            	else { int i = 0; for(; i < len - 1; i++) cout << to_string((ushort)(omem[pos + i])) << " ";
                cout << to_string((ushort)(omem[pos + i])) << endl; }
            } else cout << RED << lang[5] << RESET << endl;
        } else if(a[0] == "res" && a.size() == 2) {
            int size = stoi(a[1]);
            if(!test) omem.resize(size);
            else nmem.resize(size);
            mems = size;
        } else if(a[0] == "run" && a.size() == 2) {
            int pos = stoi(a[1]);

            if(pos < mems) {
                ecur.ss.set("pos", to_string(pos));
                TSSException te = ecur.ss.docode(ecur.code, debug);
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
        else if(a[0] == "call" && a.size() == 2) {
            ecur.code.insert(ecur.code.begin(), "exit");
            ecur.code.insert(ecur.code.begin(), "call " + a[1]);
            TSSException te = ecur.ss.docode(ecur.code, debug);
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
        else if (a[0] == "ver") {
        	cout << MAGENTA << ver;
			if(test) cout << " TEST";
			if(debug) cout << " DEBUG";
			cout << RESET << endl; }
        else if (a[0] == "save" && a.size() == 2) { savem(a[1]); }
        else if (a[0] == "load" && a.size() == 2) { loadm(a[1]); }
        else if(a[0] == "help") {
            //cout << "list\nch name\ncl\nset pos val\nprint pos len\nres size\nrun pos\ncur\ncall name\ncls\nclear\nver\nsave filename\nload filename\nexit" << endl;
            cout << "list                " << lang[10] << endl
            	 << "ch <val>            " << lang[11] << endl
            	 << "cur                 " << lang[12] << endl << endl
            	 << "cl                  " << lang[13] << endl
            	 << "set <pos> <val>     " << lang[14] << endl
            	 << "print <pos> <len>   " << lang[15] << endl
            	 << "res <len>           " << lang[16] << endl
            	 << "run <pos>           " << lang[17] << endl
            	 << "call <val>          " << lang[18] << endl
            	 << "save/load <file>    " << lang[19] << endl << endl
            	 << "cls/clear           " << lang[20] << endl
            	 << "ver                 " << lang[21] << endl
            	 << "exit                " << lang[22] << endl;
        } else { cout << YELLOW << lang[8] << RESET << endl; }
	}
}


void tss::gfunc(string name) {
	if(name == "getb") { // get bits: $pos $range var
		if(test) {
			int pos = stoi(tss::stack[0]);
			int range = stoi(tss::stack[1]);
			if(pos < mems) {
			    tss::set(
			        tss::stack[2],
			        btos(nmem.get_range(pos, range))
			    );
			} else cout << RED << "[" << ecur.name << ", getb] " << lang[5] << ": $pos" << RESET << endl;
	    } else {
	    	int pos = stoi(tss::stack[0]);
		    if(pos < mems) {
		        tss::set(
		            tss::stack[1],
		            to_string(omem[pos])
		        );
		    } else cout << RED << "[" << ecur.name << ", getb] " << lang[5] << ": $pos" << RESET << endl;
	    }
	} else if(name == "setb") { // set bits: $pos $bits
		if(test) {
			int pos = stoi(tss::stack[0]);
			if(pos < mems) {
			    nmem.set_range(pos, stob(tss::stack[1]));
			} else cout << RED << "[" << ecur.name << ", setb] " << lang[5] << ": $pos" << RESET << endl;
	    } else {
	    	int pos = stoi(tss::stack[0]);
			if(pos < mems) {
			    omem[pos] = (char)(stoi(tss::stack[1]));
			} else cout << RED << "[" << ecur.name << ", setb] " << lang[5] << ": $pos" << RESET << endl;
	    }
	} else if(name == "btoi") { // bits to integer: $bits var
		if(test) tss::set(
			tss::stack[1],
			to_string(btoi(stob(tss::stack[0])))
		);
		else cout << RED << "[" << ecur.name << ", btoi] " << lang[7] << RESET << endl;
	} else if(name == "itob") { // integer to bits: $num var
		if(test)
		tss::set(
			tss::stack[1],
			btos(itob(stoi(tss::stack[0])))
		);
		else cout << RED << "[" << ecur.name << ", itob] " << lang[7] << RESET << endl;
	} else if(name == "getn") { // get num: $pos $range var
		if(test) {
			int pos = stoi(tss::stack[0]);
			int range = stoi(tss::stack[1]);
			if(pos < mems) {
			    tss::set(
			        tss::stack[2],
			        to_string(btoi(nmem.get_range(pos, range)))
			    );
			} else cout << RED << "[" << ecur.name << ", getn] " << lang[5] << ": $pos" << RESET << endl;
	    } else cout << RED << "[" << ecur.name << ", getn] " << lang[7] << RESET << endl;
	} else if(name == "setn") { // set num: $pos $num
		if(test) {
			int pos = stoi(tss::stack[0]);
			if(pos < mems) {
			    nmem.set_range(pos, itob(stoi(tss::stack[1])));
			} else cout << RED << "[" << ecur.name << ", setn] " << lang[5] << ": $pos" << RESET << endl;
	    } else cout << RED << "[" << ecur.name << ", setn] " << lang[7] << RESET << endl;
	} else if(name == "resmem") { // resize memory: $val
        int size = stoi(tss::stack[0]);
        if(!test) omem.resize(size);
        else nmem.resize(size);
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
