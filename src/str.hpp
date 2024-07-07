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
