#include "main.hpp"

using namespace std;

int main(int argc, char **argv) {
    if(argc > 1) {
        string tmp = "";
        for(int i = 1; i < argc; i++) {
            tmp = argv[i];
            if(tmp == "--debug") debug = true;
            else if(tmp == "--test") test = true;
        }
    }
    int i = init();
    if(i == 1) {
        cout << RED << "[INIT ERROR] Didn't find language" << RESET << endl;
        return 0;
    } else if(i != 0) return 0;
    loop();
    return 0;
}
