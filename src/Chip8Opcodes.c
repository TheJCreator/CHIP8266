#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chip8inters.h"
#include "Chip8Opcodes.h"

int OpcodeExec(Chip8 *c, int pc){
	int count= 2;
	unsigned char *code = &c->memory[pc];
	unsigned char uphalf = (code[0] >> 4);
	printf("%04X %02X %02X\t", pc, code[0], code[1]);
	unsigned short addr;
	unsigned char regX, regY;
	unsigned char num;
	switch(uphalf){
		case 0x00:
			//Clear screen
			if(code[1] == 0xe0){
				printf("CLS\n");
			}
			else if(code[1] == 0xee){
				printf("RETURN\n");
			}
			else{
				printf("\n");
			}
			break;
		case 0x01:
			//GOTO
			addr = ((code[0] & 0x0f) << 8) | code[1];
			printf("GOTO $%04X\n", addr);
			break;
		case 0x02:
			//CALL
			addr = ((code[0] & 0x0f) << 8) | code[1];
			printf("CALL $%04X\n", addr);
			break;
		case 0x03:
			//COND, EQL
			regX = (code[0] & 0x0f);
			num = code[1];
			printf("IF reg%01X EQL $#%02X, JMP $%04X\n", regX, num, pc+4);
			break;
		case 0x04:
			//COND DIFF
			regX = (code[0] & 0x0f);
			num = code[1];
			printf("IF reg%01X NO-EQL $#%02X, JMP $%04X\n", regX, num, pc+4);
			break;
		case 0x05:
			//COND (W/ REGISTERS)
			regX = (code[0] & 0x0f);
			regY = code[1] & 0xf0;
			printf("IF reg%01X EQL reg%02X, JMP $%04X\n", regX, regY, pc+4);
			break;
		case 0x06:
			//ADD VALUE TO REG
			regX = (code[0] & 0x0f);
			num = code[1] & 0xf0;
			printf("reg%01X EQL $#%02X\n", regX, num);
			break;
		case 0x07:
			//ADD NUM TO REG VALUE
			regX = (code[0] & 0x0f);
			num = code[1] & 0xf0;
			printf("reg%01X ADD $#%02X\n", regX, num);
			break;
		case 0x08:
			regX = code[0] & 0x0f;
			regY = (code[1] & 0xf0) >> 4;
			switch(code[1] & 0x0f){
				case(0x00):
					//X = Y
					printf("reg%01X EQL reg%01X\n", regX, regY);
					break;
				case(0x01):
					//X = X | Y
					printf("reg%01X EQL reg%01X OR reg%01X\n",regX, regX, regY);
					break;
				case(0x02):
					//X = X & Y
					printf("reg%01X EQL reg%01X AND reg%01X\n", regX, regX, regY);
					break;
				case(0x03):
					//X = X ^ Y
					printf("reg%01X EQL reg%01X XOR reg%01X\n", regX, regX, regY);
					break;
				case(0x04):
					//X += Y
					printf("reg%01X ADD reg%01X\n", regX, regY);
					break;
				case(0x05):
					//X -= Y
					printf("reg%01X RMV reg%01X\n", regX, regY);
					break;
				case(0x06):
					//X = Y = Y >> 1
					//reg F is set to the least significant bit (i.e 01010101)
					//                                           VF =      /|\
					printf("reg%01X EQL SHR reg%01X (regF EQL %01X & 0x01)\n", regX, regY, regY);
					break;
				case(0x07):
					//X = Y - X
					printf("reg%01X SUBB reg%01X\n", regX, regY);
					break;
				case(0x0e):
					//X = Y = Y << 1
					//reg F is set to the most significant bit (i.e 01010101)
					//                                       VF = /|\
					printf("reg%01X EQL SHR reg%01X (regF EQL %01X & 0x01)\n", regX, regY, regY);
					break;
				default:
					printf("\n");
			}
			break;
		case 0x09:
			regX = code[0] & 0x0f;
			regY = (code[1] & 0xf0) >> 4;
			printf("IF reg%01X EQL reg%01X, JUMP $%04X\n", regX, regY, pc+4);
		default:
			printf("\n");
	}
	return count;
}
//1NNN
void JP(Chip8 * c, unsigned short address){
  c->pc = address;
}
//2NNN
void CALL(Chip8 * c, unsigned short address){
  c->Stack[c->SP] = c->pc;
  c->pc = address;
  c->SP++;
}
//3XKK
void SE(Chip8 * c, unsigned char reg, unsigned char val){
  if(c->V[reg] == val){
    c->pc += 2;
  }
}
//4XKK
void SNE(Chip8 * c, unsigned char reg, unsigned char val){
  if(c->V[reg] != val){
    c->pc += 2;
  }
}
//5XY0
void SRE(Chip8 * c, unsigned char regX, unsigned char regY){
  if(c->V[regX] == c->V[regY]){
    c->pc += 2;
  }
}
//6XKK
void LD(Chip8 * c, unsigned char reg, unsigned char val){
  c->V[reg] = val;
}
//7XKK
void ADD(Chip8 * c, unsigned char reg, unsigned char val){
  c->V[reg] += val;
}
//8XY0
void LRD(Chip8 * c, unsigned char regX, unsigned char regY){
  c->V[regX] = c->V[regY];
}
//8XY1
void OR(Chip8 * c, unsigned char regX, unsigned char regY){
  c->V[regX] = c->V[regX] | c->V[regY];
}
//8XY2
void AND(Chip8 * c, unsigned char regX, unsigned char regY){
  c->V[regX] = c->V[regX] & c->V[regY];
}
//8XY2
void XOR(Chip8 * c, unsigned char regX, unsigned char regY){
  c->V[regX] = c->V[regX] ^ c->V[regY];
}
//8XY4
void ADDF(Chip8 * c, unsigned char regX, unsigned char regY){
  unsigned short result = c->V[regX] + c->V[regY];
  c->V[0x0f] = (result & 0x0100) >> 8;
  c->V[regX] = result & 0x00ff;
}
//8XY5
void SUB(Chip8 * c, unsigned char regX, unsigned char regY){
  if(c->V[regX] > c->V[regY]){
    c->V[0x0f] = 0x01;
  }
  else{
    c->V[0x0f] = 0x00;
  }
  c->V[regX] -= c->V[regY];
}
//8XY6
void SHR(Chip8 * c, unsigned char reg){
  c->V[0x0f] = c->V[reg] & 1;
  c->V[reg] = c->V[reg] >> 1;
}
//8XY7
void SUBN(Chip8 * c, unsigned char regX, unsigned char regY){
  if(c->V[regX] < c->V[regY]){
    c->V[0x0f] = 0x01;
  }
  else{
    c->V[0x0f] = 0x00;
  }
  c->V[regX] = c->V[regY] - c->V[regX];
}
//8XYE
void SHL(Chip8 * c, unsigned char reg){
  c->V[0x0f] = c->V[reg] >>7;
  c->V[reg] = c->V[reg] << 1;
}
//9XY0
void SNER(Chip8 * c, unsigned char regX, unsigned char regY){
	if(c->V[regX] != c->V[regY]){
    c->pc += 2;
  }
}
//ANNN
void LD_I(Chip8 * c, unsigned short val){
	c->I = val;
}
//BNNN
void JPR(Chip8 * c, unsigned short address){
  c->pc = c->V[0] + address;
}
//CXKK
void RND(Chip8 * c, unsigned char reg, unsigned char val){
	c->V[reg] = (rand() % 256) & val;
}
//DXYN
void DRW(Chip8 * c, unsigned char regX, unsigned char regY, unsigned char n){
	for(int rows = 0; rows < n; rows++){
		unsigned char sprite = c->memory[c->I + rows];
		for(int pixel = 0; pixel < 8; pixel++){
			if((sprite & (0x80 >> pixel)) !=0){
				if(c->video[regX + (regY * 64)] == 1){
					 c->V[0x0f] = 1;
					 c->video[regX + (regY * 64)] ^= 1;
				 }
			}
		}
	}
}
