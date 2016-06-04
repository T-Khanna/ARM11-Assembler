/////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: assemble.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ARMasm.h"
#include "bitOper.h"

//-- FUNCTION DECLARATIONS ----------------------------------------------------

<<<<<<< HEAD
int32_t setDataIntrBits(int32_t result,
		int s, int opcode, int rn, int rd);
int32_t setOperand(int32_t result, int operand2);

// uncondiniotanl mask
const uint32_t uncond_mask = (1<<31) + (1<<30) + (1<<29);

//TODO: Fill these
uint32_t ASMadd(int32_t operands[]) {
  int32_t result = 0;
  //setting given intruction bits
  result = setDataIntrBits(result,0,4,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result, isshift, 25); // set 25 bit to isshift

  result = setOperand(result, operands[2]);

  return (unsigned) result;
}

uint32_t ASMsub(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,2,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result,isshift,25); // set 25 bit to isshift

  result = setOperand(result, operands[2]);

  return (unsigned) result;
}

uint32_t ASMrsb(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,3,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result,isshift,25); // set 25 bit to isshift

  result = setOperand(result, operands[2]);
  
  return (unsigned) result;
}

uint32_t ASMand(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,0,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);

  return (unsigned) result;
}

uint32_t ASMeor(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,1,operands[1],operands[0]);
   
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);
   
  return (unsigned) result;
}

uint32_t ASMorr(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,12,operands[1],operands[0]);
 
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);
       
  return (unsigned) result;
}

uint32_t ASMmov(int32_t operands[]) {
  // fix this
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,0,13,0,operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[1]);
       
  return (unsigned) result;
}

uint32_t ASMtst(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,1,8,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);
       
  return (unsigned) result;
}

uint32_t ASMteq(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,1,9,operands[1],operands[0]);

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);

  return (unsigned) result;
}

uint32_t ASMcmp(int32_t operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result,1,10,operands[1],operands[0]);
   
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2]);
       
  return (unsigned)  result;
=======
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
>>>>>>> master
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

<<<<<<< HEAD
int32_t setDataIntrBits(int32_t result,int s,
	       	int opcode, int rn, int rd){
  result |= uncond_mask;
  setBits(&result,21,&opcode,0,4);
  setBit(&result,s,20);
  setBits(&result,16,&rn,0,4);
  setBits(&result,12,&rd,0,4);
  return result;
}

int32_t setOperand(int32_t result, int operand2){
 
 if(ispunct(operand2)){
    // store operant in intruction bits
   setBits(&result, 0, &operand2, 0, 8); 
   return result;
 }

 // TODO : 
 return result;
}

uint32_t ASMldr(int32_t operands[]) {
=======
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
>>>>>>> master
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
<<<<<<< HEAD
=======





>>>>>>> master
