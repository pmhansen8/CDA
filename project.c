#include "spimcore.h"

//MySpim Final Project
//Created by:
//Damien Way
//Patrick Hansen
//Adan Rojas
//Chrystel Geno
//Alexander Nguyen

/* ALU */
/* 10 Points */
//Chrystel Geno
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero){

    //Perform's the ALU operation based on ALUControl input
    switch(ALUControl){
        //Addition
        case 0x0:
            *ALUresult = A + B;
            break;

            //Subtraction
        case 0x1:
            *ALUresult = A - B;
            break;

            //(signed) less than
        case 0x2:
            //If A < B, set ALUresult = 1; otherwise set to 0
            *ALUresult = ((int)A < (int)B) ? 1 : 0;
            break;

            //(unsigned) less than
        case 0x3:
            //If A < B, set ALUresult = 1; otherwise set to 0
            *ALUresult = (A < B) ? 1 : 0;
            break;

            //Bw AND
        case 0x4:
            *ALUresult = A & B;
            break;

            //Bw OR
        case 0x5:
            *ALUresult = A | B;
            break;

            //shift left B 16 bits
        case 0x6:
            *ALUresult = B << 16;
            break;

            //NOT A
        case 0x7:
            *ALUresult = ~A;
            break;

            //default case (error case)
        default:
            break;
    }

    //set the Zero flag if ALUresult = 0
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* Instruction Fetch */
/* 10 Points */
//Alex Nguyen
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    // Check word alignment (PC is multiple of 4)
    if(PC % 4 != 0)
        return 1; // Halt condition

    // Fetch instruction from memory at address PC
    *instruction = Mem[PC >> 2]; // PC/4 to get word index

    return 0;
}

/* Instruction Partition */
/* 10 Points */
//Damien Way
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2,
                           unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //opcode (bits 31-26)
    *op = (instruction >> 26) & 0x3F;

    //source register rs (bits 25-21)
    *r1 = (instruction >> 21) & 0x1F;

    //source register rt (bits 20-16)
    *r2 = (instruction >> 16) & 0x1F;

    //destination register rd (bits 15-11)
    *r3 = (instruction >> 11) & 0x1F;

    //(bits 5-0) for R-type instructions
    *funct = instruction & 0x3F;

    //immediate value (bits 15-0)
    *offset = instruction & 0xFFFF;

    //jump target (bits 25-0)
    *jsec = instruction & 0x03FFFFFF;
}

/* Instruction Decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    // Initialize controls to 0
    controls->RegDst = 0;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemRead = 0;
    controls->MemtoReg = 0;
    controls->ALUOp = 0;
    controls->MemWrite = 0;
    controls->ALUSrc = 0;
    controls->RegWrite = 0;

    // Set the control signals based on the opcode
    switch(op){
        // R-type instructions
        case 0x0:
            controls->RegDst = 1;      // Destination register -> rd
            controls->RegWrite = 1;    // Allows register write
            controls->ALUOp = 7;       // ALU operation determined by funct field
            break;

            // Add immediate --> 0000 1000
        case 0x8:
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = 0;
            break;

            // Load word (lw) --> 0010 0011
        case 0x23:
            controls->ALUSrc = 1;
            controls->MemtoReg = 1;    // Write data comes from the memory
            controls->RegWrite = 1;    // Enables the register write
            controls->MemRead = 1;     // Enables the memory read
            controls->ALUOp = 0;
            break;

            // Store word (sw) --> 0010 1011
        case 0x2B:
            controls->ALUSrc = 1;
            controls->MemWrite = 1;
            controls->ALUOp = 0;
            break;

            // Load upper immediate (lui) --> 0000 1111
        case 0xF:
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = 6;       // ALU performs shift left 16 bits bc it requires upper 16 bits
            break;

            // Branch on equal (beq) --> 0000 0100
        case 0x4:
            controls->Branch = 1;      //pc updates for branch
            controls->ALUOp = 1;       // ALU performs subtraction bc branch requires it
            break;

            // Set less than immediate (slti) --> 0000 1010
        case 0xA:
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = 2;       //ALU performs set less than (signed)
            break;

            // Set less than immediate unsigned (sltiu) --> 0000 1011
        case 0xB:
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = 3;       // ALU performs set less than (unsigned)
            break;

            // Jump (j) --> 0000 0010
        case 0x2:
            controls->Jump = 1;        // Enables jump
            break;

            //If none of these cases apply then the halt condition occurs, returning 1
        default:
            return 1; // Halt condition due to invalid opcode
    }

    return 0; // Successful decoding
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    // Read data from registers r1 and r2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
//Damien Way
void sign_extend(unsigned offset, unsigned *extended_value)
{
    // Check if the offset is negative
    if(offset & 0x8000)
        // if constant is a negative number: extend with 1's to the left
        *extended_value = offset | 0xFFFF0000;
    else
        // if constant is a positive number: extend with 0's to the left
        *extended_value = offset & 0x0000FFFF;
}

/* ALU Operations */
/* 10 Points */
//Chrystel Geno
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct,
                   char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    unsigned ALUControl = ALUOp;

    // Determine ALU operation for R-type instructions using funct field
    if(ALUOp == 7){
        switch(funct){
            case 0x20: //add
                ALUControl = 0;
                break;
            case 0x22: //subtract
                ALUControl = 1;
                break;
            case 0x24: //and
                ALUControl = 4;
                break;
            case 0x25: //or
                ALUControl = 5;
                break;
            case 0x2A: //set on less than (signed)
                ALUControl = 2;
                break;
            case 0x2B: //set on less than (unsigned)
                ALUControl = 3;
                break;
                //halt condition if invalid
            default:
                return 1;
        }
    }

    unsigned B = (ALUSrc == 1) ? extended_value : data2;

    // Perform ALU operation
    ALU(data1, B, ALUControl, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
//Alex Nguyen
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,
              unsigned *memdata, unsigned *Mem)
{
    /*ALUresult is an address if MemRead/MemWrite is divisible by 4 and asserted, so it
     check to see if MemRead/MemWrite is asserted beforehand*/
    if((MemRead == 1 || MemWrite == 1) && (ALUresult % 4 != 0))
        return 1; // Halt condition

    //read from the memory location addressed by the ALUresult
    if(MemRead == 1){
        *memdata = Mem[ALUresult >> 2];
    }

    //write the value of data2 to memory location given by ALUresult
    if(MemWrite == 1){
        Mem[ALUresult >> 2] = data2;
    }


    return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult,
                    char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    //Continue only if register write is enabled
    if(RegWrite == 1){
        // Determine whether the data will be written from memory or ALU result
        unsigned WriteData = (MemtoReg == 1) ? memdata : ALUresult;

        // Determine which destination register, either rd or rt
        unsigned WriteRegister = (RegDst == 1) ? r3 : r2;

        // Write that data to a selected register
        Reg[WriteRegister] = WriteData;
    }
}

/* PC Update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump,
               char Zero, unsigned *PC)
{
    //update the PC by increment of 4
    *PC += 4;

    // If branch is taken then update PC, shift left by 2
    if(Branch == 1 && Zero == 1){
        *PC += (extended_value << 2);
    }

    // If a jump instruction is requested, use the upper 4 bits of PC and left shift those bits of jsec by 2
    if(Jump == 1){
        *PC = (*PC & 0xF0000000) | (jsec << 2);
    }
}
