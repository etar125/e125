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

struct Machine
{
	std::string name;
	std::vector<byte> memory;
	byte registers = 0;
	//const byte StackPointer = 0;
	//const byte StackStart = 4;
	//const byte StackSize = 16;
	byte other = 4;
	byte position = 0;
	Extension main;

	void Clear() { std::fill(memory.begin(), memory.end(), 0); }

	Machine(Extension ext) { memory.resize(256); Clear(); main = ext; }
	Machine(Extension ext, std::string _name) { memory.resize(256); Clear(); main = ext; name = _name; }

	

	void SetMemSize(std::size_t size) { if(size != 256) { memory.resize(size); Clear(); } }

	void LoadToMemory(byte Byte, byte Position) { memory[Position] = Byte; }
 	byte GetFromMemory(byte Position) { return memory[Position]; }

	void Run(byte Position)
	{
		if(Position >= other)
		{
			main.ss.set("pos", std::to_string(Position));
			TSSException te = main.ss.docode(main.code);
			if(te.index != -1)
			{
				std::cout << "TSSException: line " << std::to_string(te.index) << " token:[ ";
				if(te.token.type == tkntp::com) std::cout << "command, ";
				else if(te.token.type == tkntp::var) std::cout << "variable, ";
				else if(te.token.type == tkntp::lab) std::cout << "label, ";
				else if(te.token.type == tkntp::val) std::cout << "value, ";
				std::cout << "\"" << te.token.val << "\" ]\n" << main.code[te.index] << std::endl;
			}
			/* будет переписано, пока оставлю

			position = Position;
			for(; position < memory.size(); position++)
			{
				

				byte opcode = GetFromMemory(position);

				

				if(opcode == Opcodes::End) break;



				else if(opcode == Opcodes::MoveA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what = GetFromMemory(position);
					LoadToMemory(what, adress);
				}
				else if(opcode == Opcodes::MoveB)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what, adress);
				}



				else if(opcode == Opcodes::AddA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 + what2, adress);
				}
				else if(opcode == Opcodes::AddB)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 + what2, adress);
				}
				else if(opcode == Opcodes::AddC)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 + what2, adress);
				}



				else if(opcode == Opcodes::SubstractA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 - what2, adress);
				}
				else if(opcode == Opcodes::SubstractB)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 - what2, adress);
				}
				else if(opcode == Opcodes::SubstractC)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 - what2, adress);
				}
				else if(opcode == Opcodes::SubstractD)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 - what2, adress);
				}



				else if(opcode == Opcodes::MultiplyA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 * what2, adress);
				}
				else if(opcode == Opcodes::MultiplyB)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 * what2, adress);
				}
				else if(opcode == Opcodes::MultiplyC)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 * what2, adress);
				}



				else if(opcode == Opcodes::DivisionA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 / what2, adress);
				}
				else if(opcode == Opcodes::DivisionB)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(GetFromMemory(position));
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 / what2, adress);
				}
				else if(opcode == Opcodes::DivisionC)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(GetFromMemory(position));
					LoadToMemory(what1 / what2, adress);
				}
				else if(opcode == Opcodes::DivisionD)
				{
					position++;
					byte adress = GetFromMemory(position);
					position++;
					byte what1 = GetFromMemory(position);
					position++;
					byte what2 = GetFromMemory(position);
					LoadToMemory(what1 / what2, adress);
				}



				else if(opcode == Opcodes::JumpA)
				{
					position++;
					byte adress = GetFromMemory(position);
					position = adress;
				}
				else if(opcode == Opcodes::JumpB)
				{
					position++;
					byte adress = GetFromMemory(GetFromMemory(position));
					position = adress;
				}



				else if(opcode == Opcodes::JumpZeroA)
				{
					position++;
					byte adress = GetFromMemory(GetFromMemory(position));
					position++;
					byte to = GetFromMemory(position);
					if(adress == 0) position = adress;
				}
				else if(opcode == Opcodes::JumpZeroB)
				{
					position++;
					byte adress = GetFromMemory(GetFromMemory(position));
					position++;
					byte to = GetFromMemory(GetFromMemory(position));
					if(adress == 0) position = adress;
				}

				else if(opcode == Opcodes::JumpNotZeroA)
				{
					position++;
					byte adress = GetFromMemory(GetFromMemory(position));
					position++;
					byte to = GetFromMemory(position);
					if(adress != 0) position = adress;
				}
				else if(opcode == Opcodes::JumpNotZeroB)
				{
					position++;
					byte adress = GetFromMemory(GetFromMemory(position));
					position++;
					byte to = GetFromMemory(GetFromMemory(position));
					if(adress != 0) position = adress;
				}
			}
			position = 0;

			*/
		}

		
	}
};