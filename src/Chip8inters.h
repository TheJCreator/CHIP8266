#ifndef Chip8inters_H
#define Chip8C8inters_H

#define MemSize 4096
#define StackSize 0x0f
#define videoW 64
#define videoH 32
#define RegisterNum 16

typedef struct Chip8{
	//Chip's memory
	unsigned char *memory;
	//Chip's Stack
	unsigned short Stack[StackSize];
	//Chip's Stack pointer
	unsigned char SP;
	//Chip's program counter
	unsigned char pc;
	//Registers
	unsigned char V[RegisterNum];
	//Chip's Index register
	unsigned short I;
	//Chip's video memory
	unsigned char *video;
	//Chip's timers
	unsigned char delay_timer;
	unsigned char sound_timer;
}Chip8;
#endif
