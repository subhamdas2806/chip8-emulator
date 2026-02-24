# CHIP-8 Emulator

A cross-platform **CHIP-8 emulator written in C++ using SDL2**, capable of running classic CHIP-8 games with accurate instruction emulation, graphics rendering, input handling, and timers.

This project was built to gain a deeper understanding of **emulator development, low-level systems, and CPU architecture**.

---

# Table of Contents

* [About](#about)
* [Features](#features)
* [Screenshots](#screenshots)
* [ROMs](#roms)
* [Project Structure](#project-structure)
* [Installation](#installation)
* [Usage](#usage)
* [Controls](#controls)
* [Implementation Details](#implementation-details)
* [Credits](#credits)
* [Author](#author)

---

# About

CHIP-8 is an interpreted programming language developed in the 1970s and is widely used as an introduction to emulator development.

This emulator replicates the original CHIP-8 virtual machine, including:

* Memory system
* Registers
* Stack
* Timers
* Graphics display
* Input handling
* Instruction execution cycle

---

# Features

* Complete CHIP-8 instruction set implementation
* SDL2-based graphics rendering
* Keyboard input support
* Accurate delay timer emulation
* ROM loading support
* Modular and readable code structure
* Tested with multiple commercial and test ROMs

---

# Screenshots

Example:

![Test ROM](images/test_rom.png)

---

# ROMs

You can try many public domain CHIP-8 games from this repository:

https://github.com/dmatlack/chip8/tree/master/roms/games

Simply download any ROM and run:

```
chip8.exe path/to/rom.ch8
```

Example games to try:

* Pong
* Tetris
* Space Invaders
* Astro Dodge
* Breakout

Opcode test ROM:

* test_opcode.ch8

---

# Project Structure

```
chip8-emulator/

chip8.cpp
chip8.h

platform.cpp
platform.h

main.cpp

roms/

build/

README.md
.gitignore
```

---

# Installation

## Requirements

* C++ Compiler (GCC, MSVC, or Clang)
* SDL2 Library

Download SDL2:

https://www.libsdl.org/

Linux:

```
sudo apt install libsdl2-dev
```

---

# Build

Compile using:

```
g++ main.cpp chip8.cpp platform.cpp -lSDL2 -o chip8
```

---

# Usage

Run the emulator:

```
chip8.exe roms/PONG.ch8
```

---

# Controls

CHIP-8 keypad is mapped to keyboard:

```
CHIP-8      Keyboard

1 2 3 C     1 2 3 4
4 5 6 D     Q W E R
7 8 9 E     A S D F
A 0 B F     Z X C V
```

---

# Implementation Details

This emulator implements:

* 4KB memory
* 16 general purpose registers
* Index register
* Program counter
* Stack and stack pointer
* Delay timer
* Sound timer (structure implemented)
* 64 × 32 pixel display buffer
* Opcode decoding using function pointer table
* ROM loading into memory at 0x200

---

# Credits

This project was developed with guidance and reference from:

Austin Morlan
https://austinmorlan.com/posts/chip8_emulator/

Cowgod's CHIP-8 Technical Reference
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

ROM repository by dmatlack
https://github.com/dmatlack/chip8

SDL2 Documentation
https://wiki.libsdl.org/

---

# Author

Subham Das

GitHub:

https://github.com/subhamdas2806

---

# Future Improvements

Planned enhancements include:

* Sound implementation
* Super CHIP-8 support
* Instruction viewer
* Save states
* GUI menu for ROM selection
* Performance improvements
