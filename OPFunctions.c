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
#include "ARMasm.h"
#include "bitOper.c"

//-- FUNCTION DECLARATIONS ----------------------------------------------------

//TODO: Fill these
uint32_t ASMadd(int32_t operands[]) {
  return 0;
}

uint32_t ASMsub(int32_t operands[]) {
  return 0;
}

uint32_t ASMrsb(int32_t operands[]) {
  return 0;
}

uint32_t ASMand(int32_t operands[]) {
  return 0;
}

uint32_t ASMeor(int32_t operands[]) {
  return 0;
}

uint32_t ASMorr(int32_t operands[]) {
  return 0;
}

uint32_t ASMmov(int32_t operands[]) {
  return 0;
}

uint32_t ASMtst(int32_t operands[]) {
  return 0;
}

uint32_t ASMteq(int32_t operands[]) {
  return 0;
}

uint32_t ASMcmp(int32_t operands[]) {
  return 0;
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



