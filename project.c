#include "spimcore.h"

int* convertDecimalToBinary(unsigned decimalInstruction);
void printIntArray(int* array, int length);
unsigned convertBinaryByRange(int* binaryInstruciton, int start, int end);

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	// int* binaryA = convertDecimalToBinary(A);
	// printIntArray(binaryA, 32);
	// int decimalA = convertBinaryIn2sComplement(binaryA);
	// printf("%d\n", decimalA);
	// free(binaryA);

	// int* binaryB = convertDecimalToBinary(B);
	// printIntArray(binaryA, 32);
	// int decimalB = convertBinaryIn2sComplement(binaryB);
	// printf("%d\n", decimalB);
	// free(binaryB);

	if (ALUControl == '0') {
		*ALUresult = A + B;
	}
	else if (ALUControl == '1') {
		*ALUresult = A - B;
	}
	else if (ALUControl == '2') {
		*ALUresult = (int) A < (int) B ? 1 : 0;
	}
	else if (ALUControl == '3') {
		*ALUresult = A < B ? 1 : 0;
	}
	else if (ALUControl == '4') {
		*ALUresult = A & B;
	}
	else if (ALUControl == '5') {
		*ALUresult = A | B;
	}
	else if (ALUControl == '6') {
		*ALUresult = B << 16;
	}
	else if (ALUControl == '7') {
		*ALUresult = ~A;
	}

	*Zero = *ALUresult == 0 ? '1' : '0';
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	printf("PC: %x\n", PC);
	if (PC % 4 != 0) {
		return 1;
	}
	*instruction = Mem[PC / 4];

	// Check if it is invalid instruction or not. Do it later.
	return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	int* binaryInstruction = convertDecimalToBinary(instruction);
	printIntArray(binaryInstruction, 32);

	*op = convertBinaryByRange(binaryInstruction, 31, 26);
	*r1 = convertBinaryByRange(binaryInstruction, 25, 21);
	*r2 = convertBinaryByRange(binaryInstruction, 20, 16);
	*r3 = convertBinaryByRange(binaryInstruction, 15, 11);
	*funct = convertBinaryByRange(binaryInstruction, 5, 0);
	*offset = convertBinaryByRange(binaryInstruction, 15, 0);
	*jsec = convertBinaryByRange(binaryInstruction, 25, 0);

	// Free memory
	free(binaryInstruction);
}

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	// Look up spreadsheet here: https://docs.google.com/spreadsheets/d/1MlP-kBKAyWNguGBK5eeJzzck4xoViNzvHRbdCSQm5YU/edit#gid=0

	// If it is R, set all the values except ALUOp
	if (op == 0) {
		controls->RegDst = '1';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '7';
		controls->MemWrite = '0';
		controls->ALUSrc = '0';
		controls->RegWrite = '1';
		return 0;
	}

	// This is J
	if (op == 2) {
		controls->RegDst = '0';
		controls->Jump = '1';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '0';
		controls->MemWrite = '0';
		controls->ALUSrc = '0';
		controls->RegWrite = '0';
		printf("Run J type instruction\n");
		return 0;
	}

	// This is add immediate
	if (op == 8) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '0';
		controls->MemWrite = '0';
		controls->ALUSrc = '1';
		controls->RegWrite = '1';
		printf("Run add immediate\n");
		return 0;
	}

	// This is load word
	if (op == 35) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '1';
		controls->MemtoReg = '1';
		controls->ALUOp = '0';
		controls->MemWrite = '0';
		controls->ALUSrc = '1';
		controls->RegWrite = '1';
		printf("Run load word\n");
		return 0;
	}

	// This is store word
	if (op == 43) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '0';
		controls->MemWrite = '1';
		controls->ALUSrc = '1';
		controls->RegWrite = '0';
		printf("Run store word\n");
		return 0;
	}

	// This is load upper immediate
	if (op == 15) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '6';
		controls->MemWrite = '0';
		controls->ALUSrc = '1';
		controls->RegWrite = '1';
		printf("Run load upper immediate\n");
		return 0;
	}

	// This is branch on equal
	if (op == 4) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '1';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '1';
		controls->MemWrite = '0';
		controls->ALUSrc = '0';
		controls->RegWrite = '0';
		printf("Run branch on equal\n");
		return 0;
	}

	// This is set less than immediate
	if (op == 10) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '2';
		controls->MemtoReg = '0';
		controls->ALUOp = '2';
		controls->MemWrite = '2';
		controls->ALUSrc = '1';
		controls->RegWrite = '1';
		printf("Run set less than immediate\n");
		return 0;
	}

	// This is set less than immediate unsigned
	if (op == 11) {
		controls->RegDst = '0';
		controls->Jump = '0';
		controls->Branch = '0';
		controls->MemRead = '0';
		controls->MemtoReg = '0';
		controls->ALUOp = '3';
		controls->MemWrite = '0';
		controls->ALUSrc = '1';
		controls->RegWrite = '1';
		printf("Run less than immediate unsigned\n");
		return 0;
	}

	return 1;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	int* binary = convertDecimalToBinary(offset);
	// printIntArray(binary, 32);
	if (binary[15] == 1) {
		int i;
		for (i = 16; i < 32; i++) {
			binary[i] = 1;
		}
	}
	// printIntArray(binary, 32);
	// How come could extended_value be unsigned? Doesn't it be signed?
	*extended_value = convertBinaryByRange(binary, 31, 0);

	free(binary);
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	char ALUControl = ' ';
	data2 = ALUSrc == '0' ? data2 : extended_value;

	// This is R type instruction
	if (ALUOp == '7') {
		switch (funct) {
			case 32:
				// add
				ALUControl = '0';
				printf("Run add\n");
				break;
			case 34:
				// subtract
				ALUControl = '1';
				printf("Run subtract\n");
				break;
			case 36:
				// and
				ALUControl = '4';
				printf("Run and\n");
				break;
			case 37:
				// or
				ALUControl = '5';
				printf("Run or\n");
				break;
			case 42:
				// set on less than
				ALUControl = '2';
				printf("Run set on less than\n");
				break;
			case 43:
				// set less than unsigned
				ALUControl = '3';
				printf("Run set less than unsigned\n");
				break;
		}
	} 
	// add immediate, load word, store word 
	else if (ALUOp == '0') {
		ALUControl = '0';
	}
	// load upper immediate
	else if (ALUOp == '6') {
		ALUControl = '6';
	}
	// branch on equal
	else if (ALUOp == '1') {
		ALUControl = '1';
	}
	// Set less than immediate
	else if (ALUOp == '2') {
		ALUControl = '2';
	}
	// Set less than immediate unsigned
	else if (ALUOp == '3') {
		ALUControl = '3';
	}

	if (ALUControl == ' ') {
		printf("ALUControl: %c\n", ALUControl);
		return 1;
	} else {
		ALU(data1, data2, ALUControl, ALUresult, Zero);
		return 0;
	}
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

	// Write to the memory
	if (MemWrite == '1') {
		if (ALUresult % 4 != 0) {
			return 1;
		}
		printf("Mem[%u] = %u\n", ALUresult, data2);

		Mem[ALUresult >> 2] = data2;
	}
	// Read from the memory
	if (MemRead == '1') {
		if (ALUresult % 4 != 0) {
			return 1;
		}
		printf("Read Mem[%u]\n", ALUresult);
		*memdata = Mem[ALUresult >> 2];
	}

	return 0;
}


/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if (RegWrite == '1') {
		unsigned writeData = MemtoReg == '1' ? memdata : ALUresult;
		unsigned writeRegister = RegDst == '0' ? r2 : r3;
		printf("Write %u to r%u\n", writeData, writeRegister);
		Reg[writeRegister] = writeData;
	}
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
//mem is stored in byte blocks 
//This is using pointer arithmatic , please go to P[0] + 4 <-- go to the 4th element in memory 2
	*PC += 4; 

	if(Zero == '1' && Branch == '1') {
		*PC += (extended_value << 2);
	}

	if(Jump == '1') { //instruction for jump is to concat [31:28] , [25:0] jsec ]
		*PC = ((*PC & 0b11110000000000000000000000000000) + (jsec << 2));
	}
}
/*************** Helper Functions ***************/

int* convertDecimalToBinary(unsigned decimalInstruction) {
	int* binaryInstruction = malloc(sizeof(int) * 32);
	// Fill binaryInstruction by 0.
	int i;
	for (i = 0; i < 32; i++) {
		binaryInstruction[i] = 0;
	}

	int index = 0;
	while (decimalInstruction > 0 && index < 32) {
		binaryInstruction[index] = decimalInstruction % 2;
		decimalInstruction /= 2;
		index++;
	}

	return binaryInstruction;
}

/**
 * Take 32 bits int array and convert it to decimal by 2's complement
 */
int convertBinaryIn2sComplement(int* num) {
	int decimal = convertBinaryByRange(num, 31, 0);

	// If it is negative number, flip them.
	if (num[31] == 1) {
		// We can convert num into decimal and do ~num, but that might cause unexpected error, so flip each element one by one here.
		int i;
		for (i = 0; i < 32; i++) {
			num[i] = num[i] == 0 ? 1 : 0;
		}
		decimal = convertBinaryByRange(num, 31, 0);
		decimal++;
		decimal *= -1;
	}
	printIntArray(num, 32);

	return decimal;
}

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
 * Takes binary instruction and convert to decimal by range. This is unsigned conversion.
 * start is the start of taking bits (inclusive) and end is end of taking bits (inclusive)
 */
unsigned convertBinaryByRange(int* binaryInstruciton, int start, int end) {
	unsigned num = 0;
	int i;
	// Don't know what representation is good for this.
	for (int i = end; i <= start; i++) {
		num += binaryInstruciton[i] << (i - end);
		// Or num += binaryInstruciton[i] << (i - end)?;
	}
	return num;
}