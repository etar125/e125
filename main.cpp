#include <iostream>
#include "main.hpp"

using byte = unsigned char;

int main()
{

	LoadToMemory(10, 4);
	LoadToMemory(128, 5);
	LoadToMemory(3, 6);
	LoadToMemory(70, 7);
	Run(4);

	std::cout << (int)(GetFromMemory(128)) << std::endl;

	return 0;
}