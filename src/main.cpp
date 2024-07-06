#include "main.hpp"

using namespace std;

int main()
{
    if(init() != 0)
    {
        cout << RED << "[INIT ERROR] Didn't find languages maybe" << RESET << endl;
        return 0;
    }

    return 0;
}
