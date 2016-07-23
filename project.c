#include "spimcore.h"
int* convertDecimalToBinary(unsigned decimalInstruction);
void printIntArray(int* array, int length);
unsigned convertInstructionToComponent(int* binaryInstruciton, int start, int end);

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	printf("%c", ALUControl);
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	*instruction = Mem[PC / 4];

	// Check if it is invalid instruction or not. Do it later.
	return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	int* binaryInstruction = convertDecimalToBinary(instruction);
	// printIntArray(binaryInstruction, 32);
	*op = convertInstructionToComponent(binaryInstruction, 31, 26);
	*r1 = convertInstructionToComponent(binaryInstruction, 25, 21);
	*r2 = convertInstructionToComponent(binaryInstruction, 20, 16);
	*r3 = convertInstructionToComponent(binaryInstruction, 15, 11);
	*funct = convertInstructionToComponent(binaryInstruction, 5, 0);
	*offset = convertInstructionToComponent(binaryInstruction, 15, 0);
	*jsec = convertInstructionToComponent(binaryInstruction, 25, 0);
}

unsigned op,	// instruction [31-26]
	r1,	// instruction [25-21]
	r2,	// instruction [20-16]
	r3,	// instruction [15-11]
	funct,	// instruction [5-0]
	offset,	// instruction [15-0]
	jsec;	// instruction [25-0]

int* convertDecimalToBinary(unsigned decimalInstruction) {
	int* binaryInstruction = malloc(sizeof(int) * 32);
	// Fill binaryInstruction by 0.
	int i;
	for (i = 0; i < 32; i++) {
		binaryInstruction[i] = 0;
	}

	int index = 0;
	while (decimalInstruction > 0) {
		binaryInstruction[index] = decimalInstruction % 2;
		decimalInstruction /= 2;
		index++;
	}

	return binaryInstruction;
}

int* convertInstructionToMachineCode(unsigned *instruction) {
	// First create an array of machine code for 
	int* machineInstruction = malloc(sizeof(int) * 32);
	return machineInstruction;
}

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	return 0;
}


/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

/*************** Helper Functions ***************/

void printIntArray(int* array, int length) {
	while (--length >= 0) {
		printf("%d", array[length]);
		if (length % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");
}

/**
 * Takes binary instruction and convert to instruction component.
 * start is the start of taking bits (inclusive) and end is end of taking the bit (inclusive)
 */
unsigned convertInstructionToComponent(int* binaryInstruciton, int start, int end) {

}