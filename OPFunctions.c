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
uint32_t ASMadd(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (4) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMsub(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (2) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMrsb(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (3) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;
 
}

uint32_t ASMand(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (0) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;
 
}

uint32_t ASMeor(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (1) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMorr(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (12) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMmov(int32_t operands[]) {
  // fix this
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (13) * 2097152; // += opcode * 2^21
  result += (0) * 1048576; // += S * 2^20
  result += (0) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[1]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMtst(int32_t operands[]) {
  uint32_t result = 0;
  result += (12<<28);// += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (8) * 2097152; // += opcode * 2^21
  result += (1) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMteq(int32_t operands[]) {
  uint32_t result = 0;
  result += (1<<31) + (1<<30) + (1<<29);  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (9) * 2097152; // += opcode * 2^21
  result += (1) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMcmp(int32_t operands[]) {
  uint32_t result = 0;
  result += (12) * 268435456;  // += cond * 2^28
  int32_t isshift = 0; // have to implement the logic for this
  result += (isshift) * 33554432; // += I * 2^25
  result += (10) * 2097152; // += opcode * 2^21
  result += (1) * 1048576; // += S * 2^20
  result += (operands[1]) * 65536; // += Rn * 2^16
  result += (operands[0]) * 4096;  // += Rd * 2^12
  int32_t operand = operands[2]; // here we implement 
  result += operand * 1;         // += operand * 2^0
  return result;

}

uint32_t ASMmul(int32_t operands[]) {
  return 0;
}

uint32_t ASMmla(int32_t operands[]) {
  return 0;
}

uint32_t ASMldr(int32_t operands[]) {
  return 0;
}

uint32_t ASMstr(int32_t operands[]) {
  return 0;
}

uint32_t ASMbeq(int32_t operands[]) {
  return 0;
}

uint32_t ASMbne(int32_t operands[]) {
  return 0;
}

uint32_t ASMbge(int32_t operands[]) {
  return 0;
}

uint32_t ASMblt(int32_t operands[]) {
  return 0;
}

uint32_t ASMbgt(int32_t operands[]) {
  return 0;
}

uint32_t ASMble(int32_t operands[]) {
  return 0;
}

uint32_t ASMb(int32_t operands[]) {
  return 0;
}

uint32_t ASMlsl(int32_t operands[]) {
  return 0;
}

uint32_t ASMandeq(int32_t operands[]) {
  return 0;
  // this is actuallz right, dont change it 
}

