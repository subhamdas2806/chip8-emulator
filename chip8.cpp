#include "chip8.h"
using namespace std;

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

// CYcle
void Chip8::Cycle()
{
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	pc +=2;

	DecodeAndExecute();

	if(delayTimer > 0)
		--delayTimer;

	if (soundTimer > 0)
		--soundTimer;
}

//SWITCH CASES AAAAAH
void Chip8::DecodeAndExecute()
{
	 switch(opcode & 0xF000)
	 {
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					OP_00E0();
					break;
				case 0x000E:
					OP_00EE();
					break;
				default:
					break;
			}break;

		case 0x1000:
			OP_1nnn();
			break;
		case 0x2000:
			OP_2nnn();
			break;
		case 0x3000:
			OP_3xkk();
			break;
		case 0x4000:
			OP_4xkk();
			break;
		case 0x5000:
			OP_5xy0();
			break;
		case 0x6000:
			OP_6xkk();
			break;
		case 0x7000:
			OP_7xkk();
			break;
		case 0x8000:
		switch (opcode & 0x000F)
			{
				case 0x0000:
					OP_8xy0();
					break;
				case 0x0001:
					OP_8xy1();
					break;
				case 0x0002:
					OP_8xy2();
					break;
				case 0x0003:
					OP_8xy3();
					break;
				case 0x0004:
					OP_8xy4();
					break;
				case 0x0005:
					OP_8xy5();
					break;
				case 0x0006:
					OP_8xy6();
					break;
				case 0x0007:
					OP_8xy7();
					break;
				case 0x000E:
					OP_8xyE();
					break;
			}break;

		case 0x9000:
	 		OP_9xy0();
			break;
		case 0xA000:
	 		OP_Annn();
			break;
		case 0xB000:
	 		OP_Bnnn();
			break;
		case 0xC000:
	 		OP_Cxkk();
			break;
		case 0xD000:
	 		OP_Dxyn();
			break;
		case 0xE000:
	 		switch(opcode & 0x00FF)
			{
				case 0x009E:
					OP_Ex9E();
					break;
				case 0x00A1:
					OP_ExA1();
					break;
			}break;

		case 0xF000:
			switch(opcode & 0x00FF)
			{
				case 0x0007:
					OP_Fx07();
					break;
				case 0x000A:
					OP_Fx0A();
					break;
				case 0x0015:
					OP_Fx15();
					break;
				case 0x0018:
					OP_Fx18();
					break;
				case 0x001E:
					OP_Fx1E();
					break;
				case 0x0029:
					OP_Fx29();
					break;
				case 0x0033:
					OP_Fx33();
					break;
				case 0x0055:
					OP_Fx55();
					break;
				case 0x0065:
					OP_Fx65();
					break;
			}break;

	 }
	 	
}

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


//uint8_t means unsigned 8bit integer

void Chip8::OP_00E0() //00E0 means  clear screen(not a build in function)
{
    memset(video,0,sizeof(video)); //sets all puxels to 0 that is black screen
}

void Chip8::OP_00EE() //00EE means return to subroutine
{
    --sp;  //Move the bookmark pointer back one step.Because earlier, when you jumped, stack pointer moved forward.Now you're undoing that. 
    pc = stack[sp]; //Go back to the saved location. Now CPU continues from there.
}

void Chip8::OP_1nnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

void Chip8::OP_2nnn() // save the bookmark location
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

	if (registers[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip8::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

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
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] = registers[Vy];
}

void Chip8::OP_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] |= registers[Vy];
}

void Chip8::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] &= registers[Vy];
}

void Chip8::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint16_t sum = registers[Vx] + registers[Vy];
    if (sum > 255U)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vx] > registers[Vy])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] -= registers[Vy];
}

void Chip8::OP_8xy6()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = (registers[Vx] & 0x1u);
    registers[Vx] >>=1;
}

void Chip8::OP_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vy] > registers[Vx])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8xyE()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;
    registers[Vx] <<= 1;
}

void Chip8::OP_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vx] != registers[Vy])
    {
        pc +=2;
    }
}

void Chip8::OP_Annn()
{
    uint16_t address = opcode & 0x0FFFu;
    index = address;
}

void Chip8::OP_Bnnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = registers[0] + address;
}

void Chip8::OP_Cxkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;
    registers[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u; //extract x pos
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; //extract y pos
    uint8_t height = opcode & 0x000Fu; //extract height

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT; // "%" prevents to spill out the screen 

    registers[0xF] = 0; // collision happend or not(0 means didnt)

    for(unsigned int row = 0; row < height; ++row) // iterate through rows
    {
        uint8_t spriteByte = memory[index + row]; // gets the sprite byte from memoryy
        for(unsigned int col = 0;col < 8 ; ++col)//iterate through columns
        {
            uint8_t spritePixel = spriteByte & (0x80u >> col); //checks every bit, if NOT 0 means pixel on
            uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)]; 

            if(spritePixel)
            {
                if(*screenPixel == 0xFFFFFFFF)
                {
                    registers[0xF] = 1;
                }

                *screenPixel^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_Ex9E()
{
    uint8_t  Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];
    if (keypad[key])
    {
        pc += 2;
    }
}

void Chip8::OP_ExA1()
{
    uint8_t  Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];
    if (!keypad[key])
    {
        pc += 2;
    }
}

void Chip8::OP_Fx07()
{
    uint8_t  Vx = (opcode & 0x0F00u) >> 8u;
    registers[Vx] = delayTimer;
}

void Chip8::OP_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0])
	{
		registers[Vx] = 0;
	}
	else if (keypad[1])
	{
		registers[Vx] = 1;
	}
	else if (keypad[2])
	{
		registers[Vx] = 2;
	}
	else if (keypad[3])
	{
		registers[Vx] = 3;
	}
	else if (keypad[4])
	{
		registers[Vx] = 4;
	}
	else if (keypad[5])
	{
		registers[Vx] = 5;
	}
	else if (keypad[6])
	{
		registers[Vx] = 6;
	}
	else if (keypad[7])
	{
		registers[Vx] = 7;
	}
	else if (keypad[8])
	{
		registers[Vx] = 8;
	}
	else if (keypad[9])
	{
		registers[Vx] = 9;
	}
	else if (keypad[10])
	{
		registers[Vx] = 10;
	}
	else if (keypad[11])
	{
		registers[Vx] = 11;
	}
	else if (keypad[12])
	{
		registers[Vx] = 12;
	}
	else if (keypad[13])
	{
		registers[Vx] = 13;
	}
	else if (keypad[14])
	{
		registers[Vx] = 14;
	}
	else if (keypad[15])
	{
		registers[Vx] = 15;
	}
	else
	{
		pc -= 2;
	}
}

void Chip8::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	delayTimer = registers[Vx]; //sets the delayTimer 
}

void Chip8::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	soundTimer = registers[Vx]; // sets the soundTimer 
}

void Chip8::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	index += registers[Vx]; //adds up the previous registers
}

void Chip8::OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = registers[Vx];
	index =  FONTSET_START_ADDRESS + (5 * digit); // set index to the memory address of the sprite for the digit in Vx(idek what that means honestly)
}

void Chip8::OP_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t  value = registers[Vx];

	memory[index +2] = value % 10;
	value /= 10;

	memory[index +1] = value % 10;
	value /= 10;

	memory[index] = value % 10;
}

void Chip8::OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		memory[index + i] = registers[i]; //Store registers V0 through Vx in memory starting at location I
	}
}

void Chip8::OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		registers[i] = memory[index + i]; //Store registers V0 through Vx in memory starting at location I
	}
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