#pragma once
#include <cstdint>
#include <random>
#include <chrono>
#include <fstream>
#include <cstring>
using namespace std;

class Chip8
{
public:

    Chip8();

    void Cycle();

    void OP_00E0();
    void OP_00EE();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xkk();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();

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