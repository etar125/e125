#pragma once

#include <string>
#include <vector>
using namespace std;

bool strs(string s, string w) {
    for(int i = 0; i < s.length() && i < s.length(); i++)
        if(s[i] != w[i]) return false;
    return true;
}

vector<string> strsp(string s, char w) {
    vector<string> res;
    string temp;
    for(char a : s) {
        if(a == w) {
            res.push_back(temp);
            temp = "";
        }
        else temp += a;
    }
    res.push_back(temp);
    return res;
}

#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define MAGENTA "\033[95m"
#define RESET "\033[0m"