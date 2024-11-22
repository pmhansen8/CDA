#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    switch (ALUControl)
    {
    // Addition
    case 0:
        *ALUresult = A + B;
        break;

    // Subtraction
    case 1:
        *ALUresult = A - B;
        break;

    // Less than
    case 2:
        if ((int)A < (int)B)
        {
            *ALUresult = 1;
        }
        else
        {
            *ALUresult = 0;
        }
        break;

    // Less than (unsigned)
    case 3:
        if (A < B)
        {
            *ALUresult = 1;
        }
        else
        {
            *ALUresult = 0;
        }
        break;

    // AND
    case 4:
        *ALUresult = (A & B);
        break;

    // OR
    case 5:
        *ALUresult = (A | B);
        break;

    // If A is less than 0
    case 6:
        if ((int)A < 0)
        {
            *ALUresult = 1;
        }
        else
        {
            *ALUresult = 0;
        }
        break;

    // NOT
    case 7:
        *ALUresult = ~A;
        break;
    }

    // Set Zero based on the results received. Set 1 if *ALUResult is zero, or else 0
    if (*ALUresult == 0)
    {
        *Zero = 1;
    }
    else
    {
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
// Alex Nguyen
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    // Checking for word alignment

    if (PC % 4 != 0)
    {
        return 1;
    }
    // "Return 1 if a halt condition occurs; otherwise, return 0."

    // "Fetch the instruction addressed by PC from Mem and write it to instruction."

    *instruction = Mem[PC >> 2];
    return 0;

    // Hint said use "PC >> 2" to get the array index.
}

/* instruction partition */
/* 10 Points */
// Damien Way
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    unsigned int instructionMover = instruction;
    *op = (instructionMover & 0xFC000000) >> 26;
    instructionMover = instructionMover << 6;
    *r1 = (instructionMover & 0xF8000000) >> 27;
    *jsec = (instructionMover & 0xFFFFFFC0) >> 6;
    instructionMover = instructionMover << 5;
    *r2 = (instructionMover & 0xF8000000) >> 27;
    instructionMover = instructionMover << 5;
    *r3 = (instructionMover & 0xF8000000) >> 27;
    *offset = (instructionMover & 0xFFFF0000) >> 16;
    instructionMover = instructionMover << 10;
    *funct = (instructionMover & 0xF8000000) >> 27;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{

    controls->RegDst = 2;
    controls->Jump = 2;
    controls->Branch = 2;
    controls->MemRead = 2;
    controls->MemtoReg = 2;
    controls->ALUOp = 2;
    controls->MemWrite = 2;
    controls->ALUSrc = 2;
    controls->RegWrite = 2;

    switch (op)
    {
    case 0x00:
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 7;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
        break;

    case 0x23:
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;

    case 0x2B:
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
        break;

    case 0x04:
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 1;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
        break;

    case 0x0A:
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 2;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
        break;

    case 0x0B:
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 3;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
        break;

    case 0x0C:
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 4;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
        break;

    case 0x0D:
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 5;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
        break;

    case 0x0F:
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 6;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
        break;

    default:
        return 1;
    }

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
// Damien Way
void sign_extend(unsigned offset, unsigned *extended_value)
{
    if (offset > 0)
    {
        *extended_value = (offset & 0x0000FFFF);
    }
    else
    {
        *extended_value = (offset | 0xFFFF0000);
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    // Checks if ALUSrc is 0, if so, then operation will use R Instructions
    if (ALUSrc == 0)
    {

        switch (funct)
        {
        // Unsigned Addition
        case 0x21:
            ALUOp = 0;
            break;

            // Unsigned Subtraction
        case 0x23:
            ALUOp = 1;
            break;

            // AND
        case 0x24:
            ALUOp = 4;
            break;

            // Set Less Than
        case 0x2a:
            ALUOp = 2;
            break;

            // Set Unsigned Less Than
        case 0x2b:
            ALUOp = 3;
            break;

            // Halts on invalid
        default:
            return 1;
        }

        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    // Checks if ALUSrc is 1, if so, then operation will use I Instructions
    else if (ALUSrc == 1)
    {
        ALU(data1, extended_value, ALUOp, ALUresult, Zero); // extended value passed through ALU instead of data2
    }
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
// Alex
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    // "If MemWrite = 1, write to memory."
    if(ALUresult % 4 != 0)
    {
        return 1;
    }
    if (MemWrite == 1)
    {
        Mem[ALUresult >> 2] = data2;
        return 0;
    }

    // "If MemRead = 1, read from memory."

    if (MemRead == 1)
    {
        *memdata = Mem[ALUresult >> 2];
        return 0;
    }

    // For other outcomes = error
    if (MemRead == 1 && MemWrite == 1)
    {
        return 1;
    }
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    if (RegWrite == 0) return;
    else{
        unsigned write_register;
        unsigned data;

        if(RegDst == 0){
            write_register = r2;
        } else {
            write_register = r3;
        }

        if (MemtoReg == 0) {
            data = ALUresult;
        } else {
            data = memdata;
        }

        if(write_register != 0) Reg[write_register] = data;
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC += 4;
    if (Branch == 1 && Zero == 1) *PC += (extended_value << 2);
    if (Jump == 1) *PC = (jsec << 2);
}
