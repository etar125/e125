#include <iostream>
#include <vector>
#include <fstream>
#include "machine.hpp"

std::string version = "2.24.3_21";
std::string green = "\033[32m";
std::string red = "\033[31m";
std::string yellow = "\033[33m";
std::string lblue = "\033[94m";
std::string reset = "\033[0m";

using byte = unsigned char;

std::vector<Machine> list;

int current = 0;

void LoadCode(Machine pc, std::vector<byte> code, byte appstart)
{
	byte count = 0;
	for(int i = 1; i < code.size(); i++)
	{
		pc.LoadToMemory(code[i], appstart + count);
		count++;
	}
}

byte LoadFile(std::string s)
{
	std::ifstream file(s);
	if(!file) std::cout << red << "File does not exist" << reset << std::endl;
	else
	{
		byte appstart = 0;
		std::vector<byte> cod;
		std::string temp;
		while(std::getline(file, temp)) { for(byte n : temp) { cod.push_back(n); } }
		appstart = cod[0];
		cod.erase(cod.begin());
		LoadCode(list[current], cod, appstart);
		return appstart;
	}
	return -1;
}

void LoadRun(std::string s)
{
	list[current].Run(LoadFile(s));
}

void Logo()
{
	std::cout << " __  /   _\n/   /|/\\/\n|__  | /|\n|    |/  \\\n\\__  ||__/" << std::endl;
}

int findm(std::string name)
{
	for(int i = 0; i < list.size(); i++)
	{
		if(list[i].name == name)
			return i;
	}
	return -1;
}

void New(std::string name)
{
	if(findm(name) == -1) list.push_back(Machine(name));
	else std::cout << yellow << "Machine with that name has already been created" << reset << std::endl;
}

void Delete(std::string name)
{
	int ind = findm(name);
	if(ind != -1)
	{
		list.erase(list.begin() + ind);
		if(list.size() == 0) current = -1;
		else current = 0;
	}
	else std::cout << red << "Machine with that name does not exist" << reset << std::endl;
}

void Choose(std::string name)
{
	int ind = findm(name);
	if(ind != -1) current = ind;
	else std::cout << red << "Machine with that name does not exist" << reset << std::endl;
}

void Init()
{
	std::cout << "Initialization..." << std::endl;
	New("DEFAULT");
	Choose("DEFAULT");

	// тут должна быть проверка установленных расширений, но... их пока нет

	std::cout << "To see all commands, type \"help\"" << std::endl;
}

void Do(std::string command, std::vector<std::string> args )
{
	if(command == "new")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else New(args[0]);
	}
	else if(command == "del")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else Delete(args[0]);
	}
	else if(command == "list") for(Machine a : list) std::cout << a.name << std::endl;
	else if(command == "set")
	{
		if(args.size() != 2) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else
		{
			byte at = (byte)(std::stoi(args[0]));
			byte val = (byte)(std::stoi(args[1]));
			if(at < 4) std::cout << yellow << "Register(0-3) value cannot be changed" << reset << std::endl;
			else list[current].LoadToMemory(val, at);
		}
	}
	else if(command == "clear") std::cout << "\x1B[2J\x1B[H";
	else if(command == "clearc") list[current].Clear();
	else if(command == "ch")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else Choose(args[0]);
	}
	else if(command == "run")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else
		{
			byte at = (byte)(std::stoi(args[0]));
			list[current].Run(at);
		}
	}
	else if(command == "load")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else LoadFile(args[0]);
	}
	else if(command == "loadr")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else LoadRun(args[0]);
	}
	else if(command == "print")
	{
		if(args.size() == 1) std::cout << std::to_string(list[current].GetFromMemory((byte)(std::stoi(args[0])))) << std::endl;
		else if(args.size() == 2)
		{
			byte start = (byte)(std::stoi(args[0]));
			byte len = (byte)(std::stoi(args[1]));
			byte end = start + len;
			std::cout << "[ ";
			for(start; start < end; start++) std::cout << std::to_string(list[current].GetFromMemory(start)) << ", ";
			std::cout << " ]" << std::endl;
		}
		else std::cout << red << "Wrong arguments!" << reset << std::endl;
	}
	else if(command == "help")
	{
		std::cout << 	"new <value>           Creates new machine\n" <<
						"del <value>           Deletes machine\n" <<
						"list                  Shows all machines\n" <<
						"set <index> <value>   Sets value\n" <<
						"clear                 Clears screen\n" <<
						"clearc                Clears machine code\n" <<
						"ch <value>            Chooses machine\n" <<
						"run <index>           Runs machine code\n\n" <<
						"load <value>          Loads file\n" <<
						"loadr <value>         Loads file and runs it\n\n" <<
						"print <index>         Prints value\n" <<
						"print <index> <len>   Prints values" << std::endl;
	}
	else if(command == "ver" || command == "version") std::cout << lblue << version << reset << std::endl;
}