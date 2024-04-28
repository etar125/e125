#include <iostream>
#include <vector>
#include <fstream>

#include "machine.hpp"
#include "extension.hpp"

std::string version = u8"2.24.4_9";
std::string green = "\033[32m";
std::string red = "\033[31m";
std::string yellow = "\033[33m";
std::string lblue = "\033[94m";
std::string reset = "\033[0m";

using byte = unsigned char;

std::vector<Machine> list;
std::vector<Extension> elist;

std::vector<std::string> lcl;

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
	if(!file) std::cout << red << lcl[0] << reset << std::endl;
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
	std::cout << u8"___\n|   _\n|  /| _ __\n|-- |// \\_\n|   |/   /\n|__ |\\_ /\n" << std::endl;
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
	else std::cout << yellow << lcl[1] << reset << std::endl;
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
	else std::cout << red << lcl[2] << reset << std::endl;
}

void Choose(std::string name)
{
	int ind = findm(name);
	if(ind != -1) current = ind;
	else std::cout << red << lcl[2] << reset << std::endl;
}

void LangInit()
{
	std::string lc = "";
	std::ifstream floc("locale/cfg");
	if(!floc) lc = "en";
	else { while(std::getline(floc, lc)) {} }
	std::ifstream loc("locale/" + lc);
	if(!loc) std::cout << u8"Not found default locale!" << std::endl;
	else
	{
		std::string tmp;
		while(std::getline(loc, tmp)) lcl.push_back(tmp);
	}
}

void Init()
{
	LangInit();
	std::cout << lcl[3] << std::endl;
	New(u8"DEFAULT");
	Choose(u8"DEFAULT");

	std::ifstream exts(u8"extensions");

	if(!exts) std::cout << lcl[4] << std::endl;
	else
	{
		std::cout << lcl[5] << std::endl;
		std::string temp;
		std::ifstream ext;
		int countt = 0;
		while(std::getline(exts, temp))
		{
			ext = std::ifstream(temp);
			if(!ext) std::cout << red << lcl[6] << u8" \"" << temp << u8"\"" << reset << std::endl;
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
		std::cout << std::to_string(countt) << u8" " << lcl[7] << std::endl;
	}

	std::cout << lcl[8] << u8" \"help\"" << std::endl;
}

void RunE(std::string name)
{
	bool find = false;
	for(Extension a : elist) { if(a.name == name) { a.Run(); find = true; } }
	if(!find) std::cout << red << lcl[6] << u8" \"" << name << "\"" << reset << std::endl;
}

void Do(std::string command, std::vector<std::string> args )
{
	if(command == u8"new")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else New(args[0]);
	}
	else if(command == u8"del")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else Delete(args[0]);
	}
	else if(command == u8"list") for(Machine a : list) std::cout << a.name << std::endl;
	else if(command == u8"set")
	{
		if(args.size() != 2) std::cout << red << lcl[9] << reset << std::endl;
		else
		{
			byte at = (byte)(std::stoi(args[0]));
			byte val = (byte)(std::stoi(args[1]));
			if(at < 4) std::cout << yellow << lcl[10] << reset << std::endl;
			else list[current].LoadToMemory(val, at);
		}
	}
	else if(command == u8"clear") std::cout << "\x1B[2J\x1B[H";
	else if(command == u8"clearc") list[current].Clear();
	else if(command == u8"ch")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else Choose(args[0]);
	}
	else if(command == u8"run")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else
		{
			byte at = (byte)(std::stoi(args[0]));
			list[current].Run(at);
		}
	}
	else if(command == u8"load")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else LoadFile(args[0]);
	}
	else if(command == u8"loadr")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else LoadRun(args[0]);
	}
	else if(command == u8"print")
	{
		if(args.size() == 1) std::cout << std::to_string(list[current].GetFromMemory((byte)(std::stoi(args[0])))) << std::endl;
		else if(args.size() == 2)
		{
			byte start = (byte)(std::stoi(args[0]));
			byte len = (byte)(std::stoi(args[1]));
			byte end = start + len;
			std::cout << u8"[ ";
			for(; start < end - 1; start++) std::cout << std::to_string(list[current].GetFromMemory(start)) << u8", ";
			start++; std::cout << std::to_string(list[current].GetFromMemory(start));
			std::cout << u8" ]" << std::endl;
		}
		else std::cout << red << lcl[9] << reset << std::endl;
	}

	else if(command == u8"elist") for(Extension a : elist) std::cout << a.name << "   " << a.desc << std::endl;
	else if(command == u8"erun")
	{
		if(args.size() != 1) std::cout << red << lcl[9] << reset << std::endl;
		else RunE(args[0]);
	}

	else if(command == u8"help")
	{
		std::cout << 	u8"new <" << lcl[11] << u8">           " << lcl[14] << u8"\n" <<
						u8"del <" << lcl[11] << u8">           " << lcl[15] << u8"\n" <<
						u8"list                  " << lcl[16] << u8"\n" <<
						u8"set <" << lcl[12] << u8"> <" << lcl[11] << u8">   " << lcl[17] << u8"\n" <<
						u8"clear                 " << lcl[18] << u8"\n" <<
						u8"clearc                " << lcl[19] << u8"\n" <<
						u8"ch <" << lcl[11] << u8">            " << lcl[20] << u8"\n" <<
						u8"run <" << lcl[12] << u8">           " << lcl[21] << u8"\n\n" <<
						u8"load <" << lcl[11] << u8">          " << lcl[22] << u8"\n" <<
						u8"loadr <" << lcl[11] << u8">         " << lcl[23] << u8"\n\n" <<
						u8"print <" << lcl[12] << u8">         " << lcl[24] << u8"\n" <<
						u8"print <" << lcl[12] << u8"> <" << lcl[13] << u8">   " << lcl[25] << u8"\n\n" <<
						u8"elist                 " << lcl[26] << u8"\n" <<
						u8"erun <" << lcl[11] << u8">          " << lcl[27] << u8"\n\n" <<
						u8"exit                  " << lcl[28] << u8"" << std::endl;
	}
	else if(command == u8"ver" || command == u8"version") std::cout << lblue << version << reset << std::endl;
}