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



 // store operant in intruction bits
 // 162   setBits(&result, 0, &operand, 0, 8);
