#include <iostream>
#include "main.hpp"

using byte = unsigned char;

int main()
{

	LoadToMemory(33, 4);
	LoadToMemory(9, 5);
	LoadToMemory(21, 6);
	LoadToMemory(70, 7);
	LoadToMemory(70, 8);
	Run(4);

	std::cout << (int)(GetFromMemory(9)) << std::endl;

	LoadToMemory(43, 4);
	Run(4);

	std::cout << (int)(GetFromMemory(9)) << std::endl;

	return 0;
}