#include <iostream>

using byte = unsigned char;

byte memory[1024] : 0;
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
	SubstractA = 40, // ^^^
	SubstractB = 41,
	SubstractC = 42,
	SubstractD = 43,
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

			else if(opcode == Opcodes::Move)
			{
				position++;
				byte adress = GetFromMemory(position);
				position++;
				byte what = GetFromMemory(position);
				LoadToMemory(what, adress);
			}
		}
		position = 0;
	}
}