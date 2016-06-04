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
#include "ARMasm.h"
#include "bitOper.h"

//-- FUNCTION DECLARATIONS ----------------------------------------------------

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
}

uint32_t ASMmul(int32_t operands[]) {

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

  //setting bits of Rd using operands
  setBits(&result, 16, &operands[0], 0, 4);
  
  //setting bits of Rm using operands
  setBits(&result, 0, &operands[1], 0, 4);

  //setting bits of Rs using operands
  setBits(&result, 8, &operands[2], 0, 4);

  //bits of Rn will be 0 as result is initialised
  //bits 22-27 will be 0 as result is initialised

  return result;

}

uint32_t ASMmla(int32_t operands[]) {

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

  //setting bits of Rd using operands
  setBits(&result, 16, &operands[0], 0, 4);
  
  //setting bits of Rm using operands
  setBits(&result, 0, &operands[1], 0, 4);

  //setting bits of Rs using operands
  setBits(&result, 8, &operands[2], 0, 4);

  //setting bits of Rn using operands
  setBits(&result, 12, &operands[3], 0, 4);

  //bits 22-27 will be 0 as result is initialised

  return result;

}

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
  return 0;
}

uint32_t ASMstr(int32_t operands[]) {
  return 0;
}

uint32_t ASMlsl(int32_t operands[]) {
//lsl Rn, <#expression> as though it were mov Rn, Rn, lsl <#expression>
  
  //finding out suitable operands to call mov from operands specified
  int32_t operands_for_mov[4] = {operands[0], operands[0], atoi("lsl"),
                                 operands[1]};

  //redirecting to mov with suitable operands
  return ASMmov(operands_for_mov);

}

uint32_t ASMandeq(int32_t operands[]) {
  //compiles the instruction andeq r0, r0, r0 to the binary value 0x00000000
  //TODO: not sure if i have to check all 3 operands to be r0's here...
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

uint32_t branch_mask = 0x0a000000;

uint32_t compute_cond(enum mnemonic_code cond) {
  cond -= 16;
  cond <<= 28;
  return cond + branch_mask;
}

uint32_t ASMbeq(int32_t operands[]) {
  return compute_cond(BEQ);
}

uint32_t ASMbne(int32_t operands[]) {
  return compute_cond(BNE);
}

uint32_t ASMbge(int32_t operands[]) {
  return compute_cond(BGE);
}

uint32_t ASMblt(int32_t operands[]) {
  return compute_cond(BLT);
}

uint32_t ASMbgt(int32_t operands[]) {
  return compute_cond(BGT);
}

uint32_t ASMble(int32_t operands[]) {
  return compute_cond(BLE);
}

uint32_t ASMb(int32_t operands[]) {
  return compute_cond(B);
}
