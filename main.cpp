#include <iostream>
#include <vector>
#include <fstream>
#include "main.hpp"

using byte = unsigned char;

void RunPC(PC pc, std::vector<byte> code, bool wm)
{
	int count = 0;
	int appstart = (int)(code[0]);
	for(int i = 1; i < code.size(); i++)
	{
		pc.LoadToMemory(code[i], appstart + count);
		count++;
	}
	pc.Run(appstart);
	if(wm)
	{
		std::string name = "mem-" + std::to_string(code.size());
		std::ofstream file(name);
		std::string result;
		for(byte n : pc.memory)
		{
			result += n;
		}
		file << result;
	}
}

int main(int argc, char *argv[])
{
	std::vector<byte> code = 
	{
		4,
		32,
		9,
		21,
		1
	};
	std::vector<std::string> files;
	std::vector<std::vector<byte>> codes;
	bool write_memory = false;
	bool onepc = false;
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			std::string arg = argv[i];
			if (arg == "-w") write_memory = true;
			else if (arg == "-o") onepc = true;
			else files.push_back(arg);
		}
	}
	if(files.size() == 0) codes.push_back(code);
	else
	{
		for (std::string s : files)
		{
			std::ifstream file(s);
			if(!file) std::cout << "Not found file " << s << std::endl;
			else
			{
				std::vector<byte> cod;
				std::string temp;
				while(std::getline(file, temp))
				{
					for(byte n : temp) { cod.push_back(n); }
				}
				codes.push_back(cod);
			}
		}
	}
	if(onepc)
	{
		PC a;
		for(std::vector<byte> n : codes)
		{
			RunPC(a, n, write_memory);
		}
	}
	else
	{
		for(std::vector<byte> n : codes)
		{
			PC a;
			RunPC(a, n, write_memory);
		}
	}

	/*
	pc.LoadToMemory(32, 4);
	pc.LoadToMemory(9, 5);
	pc.LoadToMemory(21, 6);
	pc.LoadToMemory(70, 7);
	pc.LoadToMemory(70, 8);
	pc.Run(4);

	std::cout << (int)(pc.GetFromMemory(9)) << std::endl;

	pc.LoadToMemory(43, 4);
	pc.Run(4);

	std::cout << (int)(pc.GetFromMemory(9)) << std::endl;
	*/

	return 0;
}