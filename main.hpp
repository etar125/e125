#include <iostream>

using byte = unsigned char;

byte memory[1024];
const byte registers = 0;
const byte other = 4;
byte position = 0;

void LoadToMemory(byte Byte, byte Position)
{
	memory[Position] = Byte;
}

byte GetFromMemory(byte Position)
{
	return memory[Position];
}

enum Opcodes : byte
{
	MoveA = 10, // move ADR value
	MoveB = 11, // move ADR adr
	JumpA = 20, // ^^^
	JumpB = 21,
	AddA = 30, // add adr adr1 adr2
	AddB = 31, // add adr adr1 value
	AddC = 32, // add adr value adr2
	AddD = 33, // add adr value value
	SubstractA = 40, // ^^^
	SubstractB = 41,
	SubstractC = 42,
	SubstractD = 43,
	MultiplyA = 50, // ^^^
	MultiplyB = 51,
	MultiplyC = 52,
	MultiplyD = 53,
	DivisionA = 60, // ^^^
	DivisionB = 61,
	DivisionC = 62,
	DivisionD = 63,
	End = 70
};

void Run(byte Position)
{
	if(Position >= other)
	{
		position = Position;
		for(; position < 1024; position++)
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
				byte what2 = GetFromMemory(GetFromMemory(position));
				LoadToMemory(what1 + what2, adress);
			}
			else if(opcode == Opcodes::AddD)
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
		}
		position = 0;
	}
}