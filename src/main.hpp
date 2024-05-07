#include <iostream>
#include <vector>
#include <fstream>

#include "machine.hpp"
#include "extension.hpp"

std::string version = "2.24.5_7";
std::string green = "\033[32m";
std::string red = "\033[31m";
std::string yellow = "\033[33m";
std::string lblue = "\033[94m";
std::string reset = "\033[0m";

using byte = unsigned char;
using word = unsigned short;

#define bx (byte)

std::vector<Machine> list;
std::vector<Extension> elist;

int current = 0;

void tss::gfunc(std::string name)
{
    if(name == "getb") // get byte
        tss::set(tss::stack[1], std::to_string(list[current].GetFromMemory(bx(std::stoi(tss::stack[0])))));
    else if(name == "setb") // set byte
		list[current].LoadToMemory(bx(std::stoi(tss::stack[1])), bx(std::stoi(tss::stack[0])));
	else if(name == "getw") // get word
	{
		byte f = list[current].GetFromMemory(bx(std::stoi(tss::stack[0])));
		byte s = list[current].GetFromMemory(bx(std::stoi(tss::stack[1])));
		word w = f + (s << 8);
		tss::set(tss::stack[2], std::to_string(w));
	}
	else if(name == "setw") // set word
	{
		word w = list[current].GetFromMemory(bx(std::stoi(tss::stack[0])));
		byte p = list[current].GetFromMemory(bx(std::stoi(tss::stack[1])));
		byte f = w >> 8;
		byte s = (w << 8) >> 8;
		list[current].LoadToMemory(f, p);
		list[current].LoadToMemory(s, p + 1);
	}
}

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

	std::ifstream exts("extensions");

	if(!exts) std::cout << "No extensions installed" << std::endl;
	else
	{
		std::cout << "Extensions initialization..." << std::endl;
		std::string temp;
		std::ifstream ext;
		int countt = 0;
		while(std::getline(exts, temp))
		{
			ext = std::ifstream(temp);
			if(!ext) std::cout << red << "Not found \"" << temp << "\" extension"<< reset << std::endl;
			else
			{
				std::vector<std::string> ccode;
				std::string temp2;
				while(std::getline(ext, temp2)) ccode.push_back(temp2);
				std::string nname = ccode[0];
				std::string ddesc = ccode[1];
				ccode.erase(ccode.begin(), ccode.begin() + 2);
				elist.push_back(Extension(nname, ddesc, ccode));
				countt++;
			}
		}
		std::cout << std::to_string(countt) << " extensions loaded" << std::endl;
	}

	std::cout << "To see all commands, type \"help\"" << std::endl;
}

void RunE(std::string name)
{
	bool find = false;
	for(Extension a : elist) { if(a.name == name) { a.Run(); find = true; } }
	if(!find) std::cout << red << "Not found extension \"" << name << "\"" << reset << std::endl;
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
			for(; start < end - 1; start++) std::cout << std::to_string(list[current].GetFromMemory(start)) << ", ";
			start++; std::cout << std::to_string(list[current].GetFromMemory(start));
			std::cout << " ]" << std::endl;
		}
		else std::cout << red << "Wrong arguments!" << reset << std::endl;
	}

	else if(command == "elist") for(Extension a : elist) std::cout << a.name << "   " << a.desc << std::endl;
	else if(command == "erun")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else RunE(args[0]);
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
						"print <index> <len>   Prints values\n\n" <<
						"elist                 Shows all extensions\n" <<
						"erun <value>          Runs extension\n\n" <<
						"exit                  Exits" << std::endl;
	}
	else if(command == "ver" || command == "version") std::cout << lblue << version << reset << std::endl;
}