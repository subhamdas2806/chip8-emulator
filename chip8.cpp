#include "chip8.h"
using namespace std;

const unsigned int START_ADDRESS = 0x200;

const unsigned int FONTSET_SIZE = 80;
uint8_t fontset[FONTSET_SIZE] = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned int FONTSET_START_ADDRESS = 0x50;

Chip8::Chip8()
    : randGen(chrono::system_clock::now().time_since_epoch().count())
{
    pc = START_ADDRESS;

    randByte = uniform_int_distribution<uint8_t>(0,255);

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }
}

void Chip8::OP_00E0() //00E0 means  clear screen(not a build in function)
{
    memset(video,0,sizeof(video)); //sets all puxels to 0 that is black screen
}

void Chip8::OP_00EE() //00EE means return to subroutine
{
    --sp;  //Move the bookmark pointer back one step.Because earlier, when you jumped, stack pointer moved forward.Now you're undoing that. 
    pc = stack[sp]; //Go back to the saved location. Now CPU continues from there.
}

void Chip8::OP_2nnn() // save the 
{
    uint16_t address = opcode & 0x0FFFu; // gets the address to go to
    stack[sp] = pc; // saves curent location
    ++sp; //moves bookmark forward
    pc = address; // jumps to new location
}

void Chip8::OP_3xkk() //if shi then skips
{
    uint8_t Vx =  (opcode & 0x0F00u) >> 8u; // selects the registor to chec
    uint8_t byte = opcode & 0x00FFu; // this gets the number to ccompare
    if (registers[Vx] == byte)// compare
    {
        pc +=2; //if yes then increments by 2
    }
}

void Chip8::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] |= byte)
	{
		pc += 2;
	}
}

void Chip8::OP_5xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00FFu) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
    registers[Vx] = byte;
}

void Chip8::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
    registers[Vx] += byte;
}

void Chip8::OP_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00FFu) >> 4u;
    registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00FFu) >> 4u;
    registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00FFu) >> 4u;
    registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00FFu) >> 4u;
    registers[Vx] &= registers[Vy];
}

void  Chip8::LoadROM(char const* filename)
{
    ifstream file(filename,ios::binary | ios::ate); //put the pointer at the end of the file and ate means start at the end
    if (file.is_open())
    {
        streampos size = file.tellg();
        char* buffer = new char[size];//temp storage for the game ROM
        file.seekg(0,ios::beg);
        file.read(buffer,size);
        file.close();
        for(long i = 0;i<size;++i)
        {
            memory[START_ADDRESS + i]=buffer[i];
        }
        delete[] buffer;
    }
}