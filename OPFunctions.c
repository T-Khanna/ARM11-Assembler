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

//-- FUNCTION DECLARATIONS ----------------------------------------------------

//TODO: Fill these
uint32_t ASMadd(uint32_t operands[]) {
  return 0;
}

uint32_t ASMsub(uint32_t operands[]) {
  return 0;
}

uint32_t ASMrsb(uint32_t operands[]) {
  return 0;
}

uint32_t ASMand(uint32_t operands[]) {
  return 0;
}

uint32_t ASMeor(uint32_t operands[]) {
  return 0;
}

uint32_t ASMorr(uint32_t operands[]) {
  return 0;
}

uint32_t ASMmov(uint32_t operands[]) {
  return 0;
}

uint32_t ASMtst(uint32_t operands[]) {
  return 0;
}

uint32_t ASMteq(uint32_t operands[]) {
  return 0;
}

uint32_t ASMcmp(uint32_t operands[]) {
  return 0;
}

uint32_t ASMmul(uint32_t operands[]) {
  return 0;
}

uint32_t ASMmla(uint32_t operands[]) {
  return 0;
}

uint32_t ASMldr(uint32_t operands[]) {
  return 0;
}

uint32_t ASMstr(uint32_t operands[]) {
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

uint32_t ASMbeq(uint32_t operands[]) {
  return compute_cond(BEQ);
}

uint32_t ASMbne(uint32_t operands[]) {
  return compute_cond(BNE);
}

uint32_t ASMbge(uint32_t operands[]) {
  return compute_cond(BGE);
}

uint32_t ASMblt(uint32_t operands[]) {
  return compute_cond(BLT);
}

uint32_t ASMbgt(uint32_t operands[]) {
  return compute_cond(BGT);
}

uint32_t ASMble(uint32_t operands[]) {
  return compute_cond(BLE);
}

uint32_t ASMb(uint32_t operands[]) {
  return compute_cond(B);
}

uint32_t ASMlsl(uint32_t operands[]) {
  return 0;
}

uint32_t ASMandeq(uint32_t operands[]) {
  return 0;
}

