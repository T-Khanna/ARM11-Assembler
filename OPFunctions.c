/////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: assemble.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ARMasm.h"
#include "bitOper.h"

//-- FUNCTION DECLARATIONS ----------------------------------------------------

int getRegBin(char* reg){

  //removing the r in the register string gived
  reg++;

  //obtaining register number in decimal
  int reg_num = atoi(reg);

  //obtaining register number in binary
  int reg_bin = decToBin(reg_num);
  
  return reg_bin;

}


//TODO: Fill these
uint32_t ASMadd(char * operands[]) {
  return 0;
}

uint32_t ASMsub(char * operands[]) {
  return 0;
}

uint32_t ASMrsb(char * operands[]) {
  return 0;
}

uint32_t ASMand(char * operands[]) {
  return 0;
}

uint32_t ASMeor(char * operands[]) {
  return 0;
}

uint32_t ASMorr(char * operands[]) {
  return 0;
}

uint32_t ASMmov(char * operands[]) {
  return 0;
}

uint32_t ASMtst(char * operands[]) {
  return 0;
}

uint32_t ASMteq(char * operands[]) {
  return 0;
}

uint32_t ASMcmp(char * operands[]) {
  return 0;
}

uint32_t ASMmul(char * operands[]) {
  
  int32_t result = 0;

  //setting A bit 21 to 0
  setBit(&result, 0, 21); 

  //setting S bit 20 to 0
  setBit(&result, 0, 20);

  //setting Cond field to 1110
  int cond_field = 1110;
  setBits(&result, 28, &cond_field, 0, 4);

  //setting bits 4-7 to 1001
  int bits_4_7 = 1001;
  setBits(&result, 4, &bits_4_7, 0, 4);

  //setting bits of rd using operands
  int rd_bin = getRegBin(operands[0]);
  setBits(&result, 16, &rd_bin, 0, 4);
  
  //setting bits of rm using operands
  int rm_bin = getRegBin(operands[1]);
  setBits(&result, 0, &rm_bin, 0, 4);

  //setting bits of rs using operands
  int rs_bin = getRegBin(operands[2]);
  setBits(&result, 8, &rs_bin, 0 , 4);

  //bits of Rn will be 0 as result is initialised
  //bits 22-27 will be 0 as result is initialised

  return result;

  return 0;
}

uint32_t ASMmla(char * operands[]) {

  int32_t result = 0;

  //setting A bit 21 to 1
  setBit(&result, 1, 21); 

  //setting S bit 20 to 0
  setBit(&result, 0, 20);

  //setting Cond field to 1110
  int cond_field = 1110;
  setBits(&result, 28, &cond_field, 0, 4);

  //setting bits 4-7 to 1001
  int bits_4_7 = 1001;
  setBits(&result, 4, &bits_4_7, 0, 4);

  //setting bits of rd using operands
  int rd_bin = getRegBin(operands[0]);
  setBits(&result, 16, &rd_bin, 0, 4);
  
  //setting bits of rm using operands
  int rm_bin = getRegBin(operands[1]);
  setBits(&result, 0, &rm_bin, 0, 4);

  //setting bits of rs using operands
  int rs_bin = getRegBin(operands[2]);
  setBits(&result, 8, &rs_bin, 0 , 4);

  //setting bits of rn using operands
  int rn_bin = getRegBin(operands[3]);
  setBits(&result, 12, &rn_bin, 0, 4);

  //bits 22-27 will be 0 as result is initialised

  return (unsigned) result;

  return 0;
}

uint32_t ASMldr(char * operands[]) {
//instruction has syntax ldr Rd, <address>  
/*
  int32_t result = 0;
  int check = 0xFF;

  //setting L bit 20 to 1
  setBit(&result, 1, 20);   

  //setting bits 27-26 to 01
  setBit(&result, 0, 27);   
  setBit(&result, 1, 26);   

  //setting bits of rd using operands
  int rd_bin = decToBin(operands[0]);
  setBits(&result, 12, &rd_bin, 0 , 4);

  //if <address> is a numeric constant of the form: <=expression>

  
  //if <address> is a pre-indexed address specificaition
  
  //if <address> is a post-indexing addressing specification

  return (unsigned) result;
*/
  return 0;
}

uint32_t ASMstr(char * operands[]) {
//instruction has syntax str Rd, <address>  
/*  
  int32_t result = 0;

  //setting L bit 20 to 0
  setBit(&result, 0, 20);   

  //setting bits 27-26 to 01
  setBit(&result, 0, 27);   
  setBit(&result, 1, 26);   

  //setting bits of rd using operands
  int rd_bin = decToBin(operands[0]);
  setBits(&result, 12, &rd_bin, 0 , 4);

  //if <address> is a pre-indexed address specificaition
  
  //if <address> is a post-indexing addressing specification

  return (unsigned) result;
*/
  return 0;
}

uint32_t ASMlsl(char * operands[]) {
//lsl Rn, <#expression> as though it were mov Rn, Rn, lsl <#expression>

  //finding out suitable operands to call mov from operands specified
  char* operands_for_mov[4] = {operands[0], operands[0], "lsl",
                               operands[1]};

  //redirecting to mov with suitable operands
  return ASMmov(operands_for_mov);

}

uint32_t ASMandeq(char * operands[]) {

  //compiles the instruction andeq r0, r0, r0 to the binary value 0x00000000
  return 0;

}


// Branch
/* 
  For each of these instructions, there is only one operand. Due to the way
  we assigned the enum value to the function, the condition is always the
  mnemonic_code - 16. Bits 27-24 are 1010. Remaining is the 26-bit signed
  offset between the current address and the label, which should be shifted
  right two bits, only storing the lower 24 bits. This must also take into
  account the off-by-8 bytes effect from the ARM pipeline.
*/

int32_t calculate_branch(enum mnemonic_code cond, char * operands[]) {
  cond -= 16;
  int32_t offset = atoi(operands[0]);
  offset >>= 2;
  return (cond << 28) | 0x0a000000 | (offset & 0x00ffffff);
}

uint32_t ASMbeq(char * operands[]) {
  return calculate_branch(BEQ, operands);
}

uint32_t ASMbne(char * operands[]) {
  return calculate_branch(BNE, operands);
}

uint32_t ASMbge(char * operands[]) {
  return calculate_branch(BGE, operands);
}

uint32_t ASMblt(char * operands[]) {
  return calculate_branch(BLT, operands);
}

uint32_t ASMbgt(char * operands[]) {
  return calculate_branch(BGT, operands);
}

uint32_t ASMble(char * operands[]) {
  return calculate_branch(BLE, operands);
}

uint32_t ASMb(char * operands[]) {
  return calculate_branch(B, operands);
}





