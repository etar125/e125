#include "main.hpp"

using namespace std;

int main() {
    if(init() != 0) {
        cout << RED << "[INIT ERROR] Didn't find language" << RESET << endl;
        return 0;
    }
    loop();
    return 0;
}
