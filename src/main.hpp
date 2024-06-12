#include <iostream>
#include <vector>
#include <fstream>

#include "machine.hpp"
#include "extension.hpp"

std::string version = "2.24.6_12";
std::string green = "\033[32m";
std::string red = "\033[31m";
std::string yellow = "\033[33m";
std::string lblue = "\033[94m";
std::string reset = "\033[0m";

using byte = unsigned char;
using word = unsigned short;

#define bx (byte)
#define sx (byte)

std::vector<Machine> list;
std::vector<Extension> elist;

int current = 0;

void tss::gfunc(std::string name)
{
	if(name == "getb") // get byte: $position name
        tss::set(tss::stack[1], std::to_string(list[current].GetFromMemory(sx(std::stoi(tss::stack[0])))));
    else if(name == "setb") // set byte: $position $value
		list[current].LoadToMemory(bx(std::stoi(tss::stack[1])), sx(std::stoi(tss::stack[0])));
	else if(name == "getw") // get word: like getb
	{
		byte f = list[current].GetFromMemory(sx(std::stoi(tss::stack[0])));
		byte s = list[current].GetFromMemory(sx(std::stoi(tss::stack[0]) + 1));
		word w = f + (s << 8);
		tss::set(tss::stack[2], std::to_string(w));
	}
	else if(name == "setw") // set word: like setb
	{
		word w = sx(std::stoi(tss::stack[1]));
		word p = sx(std::stoi(tss::stack[0]));
		byte f = w >> 8;
		byte s = (w << 8) >> 8;
		list[current].LoadToMemory(f, p);
		list[current].LoadToMemory(s, p + 1);
	}
	else if(name == "resmem") // resize memory
		list[current].SetMemSize(std::stoi(tss::stack[0]));
	else if(name == "putstr")
	{
		std::string str = "";
		bool spec = false;
		for(char d : tss::stack[0])
		{
			if(d == '\\' && !spec) spec = true;
			else if (d == '\\' && spec) spec = false;
			else if(spec)
			{
				if(d == 'n') str += '\n';
				else if(d == 't') str += '\t';
				else if(d == 'a') str += '\033';
				spec = false;
			}
			else str += d;
		}
		std::cout << str;
	}
	else if(name == "putch")
	{
		std::cout << std::stoi(tss::stack[0]);
	}
	else if(name == "getline")
	{
		std::string str;
		getline(std::cin, str);
		tss::set(tss::stack[0], str);
	}
}

void LoadCode(Machine pc, std::vector<byte> code, short appstart)
{
	short count = 0;
	for(int i = 1; i < code.size(); i++)
	{
		pc.LoadToMemory(code[i], appstart + count);
		count++;
	}
}

word LoadFile(std::string s)
{
	std::ifstream file(s);
	if(!file) std::cout << red << "Not found file \"" << s << "\"" << reset << std::endl;
	else
	{
		short appstart = 0;
		std::vector<byte> cod;
		std::string temp;
		while(std::getline(file, temp)) { for(byte n : temp) { cod.push_back(n); } }
		appstart = cod[0] + (cod[1] << 8);
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
	std::cout << "███████╗  ███╗  ██████╗ ███████╗\n██╔════╝ ████║  ╚════██╗██╔════╝\n█████╗  ██╔██║    ███╔═╝██████╗ \n██╔══╝  ╚═╝██║  ██╔══╝  ╚════██╗\n███████╗███████╗███████╗██████╔╝\n╚══════╝╚══════╝╚══════╝╚═════╝ " << std::endl;
}

Extension GetE(std::string name)
{
	for(Extension a : elist) if(a.name == name) return a;
	std::cout << red << "Not found extension \"" << name << "\"" << reset << std::endl;
	return Extension("null");
}

void RunE(std::string name)
{
	bool find = false;
	for(Extension a : elist)
	{
		if(a.name == name)
		{
			TSSException te = a.ss.docode(a.code);
			if(te.index != -1)
			{
				std::cout << "Extension: " << name << std::endl;
				std::cout << "RunE: TSSException: index " << std::to_string(te.index) << " token:[ ";
				if(te.token.type == tkntp::com) std::cout << "command, ";
				else if(te.token.type == tkntp::var) std::cout << "variable, ";
				else if(te.token.type == tkntp::lab) std::cout << "label, ";
				else if(te.token.type == tkntp::val) std::cout << "value, ";
				std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
			}
			find = true;
		}
	}
	if(!find) std::cout << red << "Not found extension \"" << name << "\"" << reset << std::endl;
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

void New(std::string name, std::string ext)
{
	if(findm(name) == -1)
	{
		Extension e = GetE(ext);
		if(e.name != "null") list.push_back(Machine(e, name));
	}
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

	New("DEFAULT", "e125.machine8");
	Choose("DEFAULT");

	std::cout << "To see all commands, type \"help\"" << std::endl;
}

/* void savem(std::string name)
{
	int a = findm(name);
	if(a != -1)
	{
		std::ofstream file(name + ".e125machine");
		file << name + "\n";
		file << list[a].main.name + "\n";
		std::string result;
		for(byte s : list[a].memory) result += s;
		file << result;
		file.close();
	}
	else std::cout << red << "Machine with that name does not exist" << reset << std::endl;
}

void loadm(std::string path)
{
	std::ifstream file(path);
	if(!file) std::cout << red << "Not found file \"" << path << "\"" << reset << std::endl;
	else
	{
		std::vector<std::string> result;
		std::string temp;
		while(std::getline(file, temp)) result.push_back(temp);
		int ind = findm(result[0]);
		if(ind == -1) New(result[0], result[1]);
		std::vector<byte> cod;
		for(byte n : result[2]) cod.push_back(n);
		LoadCode(list[findm(result[0])], cod, 0);
	}
} */

void Do(std::string command, std::vector<std::string> args )
{
	if(command == "new")
	{
		if(args.size() != 2) std::cout << red << "Wrong arguments!" << reset << std::endl;
		else New(args[0], args[1]);
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

	/* else if(command == "savem")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		savem(args[0]);
	}
	else if(command == "loadm")
	{
		if(args.size() != 1) std::cout << red << "Wrong arguments!" << reset << std::endl;
		loadm(args[0]);
	} */

	else if(command == "help")
	{
		std::cout << 	"new <value> <ext>     Creates new machine with extension\n" <<
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
						// "savem <value>         Saves machine\n" <<
						// "loadm <value>         Loads machine\n\n" <<
						"exit                  Exits" << std::endl;
	}
	else if(command == "ver" || command == "version") std::cout << lblue << version << reset << std::endl;
}
