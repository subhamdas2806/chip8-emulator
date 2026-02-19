#include "chip8.h"
#include <fstream>
using namespace std;

const unsigned int START_ADDRESS = 0x200;
void  Chip8::LoadROM(char const* filename)
{
    ifstream file(filename,ios::binary | ios::ate);
    if (file.is_open())
    {
        streampos size = file.tellg();
        char* buffer = new char[size];
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

