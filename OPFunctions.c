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
int32_t ARMadd(int32_t operands[]) {
  return 0;
}

int32_t ARMsub(int32_t operands[]) {
  return 0;
}

int32_t ARMrsb(int32_t operands[]) {
  return 0;
}

int32_t ARMand(int32_t operands[]) {
  return 0;
}

int32_t ARMeor(int32_t operands[]) {
  return 0;
}

int32_t ARMorr(int32_t operands[]) {
  return 0;
}

int32_t ARMmov(int32_t operands[]) {
  return 0;
}

int32_t ARMtst(int32_t operands[]) {
  return 0;
}

int32_t ARMteq(int32_t operands[]) {
  return 0;
}

int32_t ARMcmp(int32_t operands[]) {
  return 0;
}

int32_t ARMmul(int32_t operands[]) {
  return 0;
}

int32_t ARMmla(int32_t operands[]) {
  return 0;
}

int32_t ARMldr(int32_t operands[]) {
  return 0;
}

int32_t ARMstr(int32_t operands[]) {
  return 0;
}

int32_t ARMbeq(int32_t operands[]) {
  return 0;
}

int32_t ARMbne(int32_t operands[]) {
  return 0;
}

int32_t ARMbge(int32_t operands[]) {
  return 0;
}

int32_t ARMblt(int32_t operands[]) {
  return 0;
}

int32_t ARMbgt(int32_t operands[]) {
  return 0;
}

int32_t ARMble(int32_t operands[]) {
  return 0;
}

int32_t ARMb(int32_t operands[]) {
  return 0;
}


int32_t ASMlsl(int32_t operands[]) {
//lsl Rn, <#expression> as though it were mov Rn, Rn, lsl <#expression>
  
  //finding out suitable operands to call mov from operands specified
  int32_t operands_for_mov[4] = {operands[0], operands[0], atoi("lsl"),
                                 operands[1]};

  //redirecting to mov with suitable operands
  return ARMmov(operands_for_mov);

}

int32_t ASMandeq(int32_t operands[]) {
  //compiles the instruction andeq r0, r0, r0 to the binary value 0x00000000
  return 0;
}

