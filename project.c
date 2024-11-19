#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl){
        //Addition
        case 0:
            *ALUresult = A+B;
            break;

        //Subtraction
        case 1:
            *ALUresult = A-B;
            break;

        //Less than
        case 2:
            if((int)A < (int)B){
                *ALUresult = 1;
            }
            else{
                *ALUresult = 0;
            }
            break;

        //Less than (unsigned)
        case 3:
            if(A < B){
                *ALUresult = 1;
            }
            else{
                *ALUresult = 0;
            }
            break;

        //AND
        case 4:
            *ALUresult = (A & B);
            break;

        //OR
        case 5:
            *ALUresult = (A | B);
            break;

        //If A is less than 0
        case 6:
            if ((int)A < 0) {
                *ALUresult = 1;
            }
            else{
                *ALUresult = 0;
            }
            break;

        //NOT
        case 7:
            *ALUresult = ~A;
            break;
    }

    //Set Zero based on the results received. Set 1 if *ALUResult is zero, or else 0
    if (*ALUresult == 0){
        *Zero = 1;
    }
    else{
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
// Alex Nguyen
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  //Checking for word alignment

  if (PC % 4 != 0) {
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
//Damien Way
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
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
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
//Damien Way
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
//Damien Way
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    //Checks if ALUSrc is 0, if so, then operation will use R Instructions
    if (ALUSrc == 0){

        switch (funct){
            //Unsigned Addition
            case 0x21:
                ALUOp = 0;
                break;

            //Unsigned Subtraction
            case 0x23:
                ALUOp = 1;
                break;

            //AND
            case 0x24:
                ALUOp = 4;
                break;

            //Set Less Than
            case 0x2a:
                ALUOp = 2;
                break;

            //Set Unsigned Less Than
            case 0x2b:
                ALUOp = 3;
                break;

            //Halts on invalid
            default:
                return 1;
        }

        ALU(data1, data2, ALUOp,ALUresult, Zero);
    }

    //Checks if ALUSrc is 1, if so, then operation will use I Instructions
    else if(ALUSrc == 1){
        ALU(data1, extended_value, ALUOp, ALUresult, Zero);//extended value passed through ALU instead of data2
    }
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
// Alex
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // "If MemWrite = 1, write to memory."
    
    if (MemWrite == 1) {
        Mem[ALUresult >> 2] = data2;
    }

    // "If MemRead = 1, read from memory."

    if (MemRead == 1) {
        *memdata = Mem[ALUresult >> 2];
    }
    
    // For other outcomes = error
    if (MemRead == 1 && MemWrite == 1) {
        return 1;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

