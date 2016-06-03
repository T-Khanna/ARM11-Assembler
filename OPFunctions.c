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
int32_t ASMadd(int32_t operands[]) {
  return 0;
}

int32_t ASMsub(int32_t operands[]) {
  return 0;
}

int32_t ASMrsb(int32_t operands[]) {
  return 0;
}

int32_t ASMand(int32_t operands[]) {
  return 0;
}

int32_t ASMeor(int32_t operands[]) {
  return 0;
}

int32_t ASMorr(int32_t operands[]) {
  return 0;
}

int32_t ASMmov(int32_t operands[]) {
  return 0;
}

int32_t ASMtst(int32_t operands[]) {
  return 0;
}

int32_t ASMteq(int32_t operands[]) {
  return 0;
}

int32_t ASMcmp(int32_t operands[]) {
  return 0;
}

int32_t ASMmul(int32_t operands[]) {
  return 0;
}

int32_t ASMmla(int32_t operands[]) {
  return 0;
}

int32_t ASMldr(int32_t operands[]) {
  return 0;
}

int32_t ASMstr(int32_t operands[]) {
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

int32_t ASMbeq(int32_t operands[]) {
  return 0;
}

int32_t ASMbne(int32_t operands[]) {
  return 0;
}

int32_t ASMbge(int32_t operands[]) {
  return 0;
}

int32_t ASMblt(int32_t operands[]) {
  return 0;
}

int32_t ASMbgt(int32_t operands[]) {
  return 0;
}

int32_t ASMble(int32_t operands[]) {
  return 0;
}

int32_t ASMb(int32_t operands[]) {
  return 0;
}

int32_t ASMlsl(int32_t operands[]) {
  return 0;
}

int32_t ASMandeq(int32_t operands[]) {
  return 0;
}

