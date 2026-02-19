#pragma once
#include <cstdint>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

class Chip8
{
    public:

    void LoadROM(char const* filename);
        uint8_t registers[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16];
        uint8_t sp{};
        uint8_t delaytimer{};
        uint8_t soundtimer{};
        uint8_t keypad[16]{};
        uint32_t video[64*32]{};
        uint16_t opcode;

    default_random_engine randGen;
    uniform_int_distribution<uint8_t> randByte;

};