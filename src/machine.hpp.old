#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../tinyss/tinyss.hpp"
#include "../tinyss/token.hpp"
#include "extension.hpp"

using byte = unsigned char;

/*

enum Opcodes : byte
{
	MoveA = 10, // move ADR value
	MoveB = 11, // move ADR adr

	JumpA = 20, // jump value
	JumpB = 21, // jump adr

	JumpZeroA = 22, // jump adr value
	JumpZeroB = 23, // jump adr adr

	JumpNotZeroA = 24, // jump adr value
	JumpNotZeroB = 25, // jump adr adr

	AddA = 30, // add adr adr1 adr2
	AddB = 31, // add adr adr1 value
	AddC = 32, // add adr value value

	SubstractA = 40, // ^^^
	SubstractB = 41,
	SubstractC = 42, // sub adr value adr2
	SubstractD = 43, // sub adr val val

	MultiplyA = 50, // like add
	MultiplyB = 51,
	MultiplyC = 52,

	DivisionA = 60, // like substract
	DivisionB = 61,
	DivisionC = 62,
	DivisionD = 63,

	//PushA = 70, // push value
	//PushB = 71, // push adr

	End = 1
};

*/

std::string tripstr(std::string str)
{

	int i = 0;
	while(str[i] == ' ' || str[i] == '\t') i++;
	return str.substr(i);
}

struct Machine
{
	std::string name;
	std::vector<byte> memory;
	short registers = 0;
	//const byte StackPointer = 0;
	//const byte StackStart = 4;
	//const byte StackSize = 16;
	short other = 4;
	short position = 0;
	Extension main;

	void Clear() { std::fill(memory.begin(), memory.end(), 0); }

	Machine(Extension ext) { memory.resize(256); Clear(); main = ext; Init(); }
	Machine(Extension ext, std::string _name) { memory.resize(256); Clear(); main = ext; name = _name; Init(); }

	void Init()
	{
		std::vector<std::string> init;
		init.push_back("call init");
		init.push_back("exit");
		bool f = false;
		for(std::string s : main.code)
		{
			if(tripstr(s) == ":init" && !f)
			{
				f = true;
				init.push_back(s);
			}
			else if(tripstr(s) == "ret" && f)
			{
				f = false;
				init.push_back(s);
			}
			else if(f)
			{
				init.push_back(s);
			}
		}
		TSSException te = main.ss.docode(init);
		if(te.index != -1)
		{
			std::cout << "Extension: " << main.name << std::endl;
			std::cout << "MachineInit: TSSException: index " << std::to_string(te.index) << " token:[ ";
			if(te.token.type == tkntp::com) std::cout << "command, ";
			else if(te.token.type == tkntp::var) std::cout << "variable, ";
			else if(te.token.type == tkntp::lab) std::cout << "label, ";
			else if(te.token.type == tkntp::val) std::cout << "value, ";
			std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
		}
	}

	void SetMemSize(std::size_t size) { if(size != 256) { memory.resize(size); Clear(); } }

	void LoadToMemory(byte Byte, short Position) { memory[Position] = Byte; }
 	byte GetFromMemory(short Position) { return memory[Position]; }

	void Run(short Position)
	{
		if(Position >= other)
		{
			main.ss.set("pos", std::to_string(Position));
			TSSException te = main.ss.docode(main.code);
			if(te.index != -1)
			{
				std::cout << "Extension: " << main.name << std::endl;
				std::cout << "MachineRun: TSSException: index " << std::to_string(te.index) << " token:[ ";
				if(te.token.type == tkntp::com) std::cout << "command, ";
				else if(te.token.type == tkntp::var) std::cout << "variable, ";
				else if(te.token.type == tkntp::lab) std::cout << "label, ";
				else if(te.token.type == tkntp::val) std::cout << "value, ";
				std::cout << "\"" << te.token.val << "\" ]\n" << te.message << std::endl;
			}
		}

		
	}
};
