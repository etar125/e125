#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int ver = 0;
int subver = 1;

int main(int argc, char *argv[])
{
	string code;
	string result;
	int appstart = 4;

	if(argc > 1)
	{
		
	}
	else
	{
		cout << "Brainfuck compiler v" << ver << "." << subver << endl;
	}
	return 0;
}