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

//-- GLOBAL VARIABLES ---------------------------------------------------------

extern int label_count;
extern uint32_t *bin_instr;
extern int num_of_lines;
extern int line_num;
extern int extra_data;

//-- FUNCTION DECLARATIONS ----------------------------------------------------

int32_t setDataIntrBits(int32_t result,
		int s, int opcode, int rn, int rd);
int32_t setOperand(int32_t result, char* str, char* shift);

// uncondiniotanl mask
const uint32_t uncond_mask = (1<<31) + (1<<30) + (1<<29);

int getRegBin(char* operand){

  //removing the r in the operandister string gived
  operand++;

  //obtaining register number in decimal
  return (int) strtol(operand, NULL, 0);

}

void getAddress(int *result, char** operands){
}

uint32_t ASMadd(char* operands[]) {
  int32_t result = 0;
  //setting given intruction bits
  result = setDataIntrBits(result, 0, ADD, getRegBin(operands[1]),
      getRegBin(operands[0]));
  
  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result, isshift, 25); // set 25 bit to isshift
 
  result = setOperand(result, operands[2], operands[3]);
 // printf("final result = %u\n",result);
  return (unsigned) result;
}

uint32_t ASMsub(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, SUB, getRegBin(operands[1]),
      getRegBin(operands[0]));
  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result,isshift,25); // set 25 bit to isshift

  result = setOperand(result, operands[2],operands[3]);

  return (unsigned) result;
}

uint32_t ASMrsb(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, RSB, getRegBin(operands[1]),
      getRegBin(operands[0]));

  int32_t isshift = 0; // have to implement the logic for this
  setBit(&result,isshift,25); // set 25 bit to isshift

  result = setOperand(result, operands[2],operands[3]);
  
  return (unsigned) result;
}

uint32_t ASMand(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, AND, getRegBin(operands[1]),
      getRegBin(operands[0]));

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);

  return (unsigned) result;
}

uint32_t ASMeor(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, EOR, getRegBin(operands[1]),
      getRegBin(operands[0]));
   
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);
   
  return (unsigned) result;
}

uint32_t ASMorr(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, ORR, getRegBin(operands[1]),
      getRegBin(operands[0]));
 
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);
       
  return (unsigned) result;
}

uint32_t ASMmov(char* operands[]) {

  // fix this
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, MOV, 0, getRegBin(operands[0]));

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[1],operands[2]);
       
  return (unsigned) result;
}

uint32_t ASMtst(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, TST, getRegBin(operands[1]),
      getRegBin(operands[0]));

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);
       
  return (unsigned) result;
}

uint32_t ASMteq(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, TEQ, getRegBin(operands[1]),
      getRegBin(operands[0]));

  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);

  return (unsigned) result;
}

uint32_t ASMcmp(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, CMP, getRegBin(operands[1]),
      getRegBin(operands[0]));
   
  int32_t isshift = 0; // have to implement the logic for this
  result |= (isshift) << 25; // += I * 2^25

  result = setOperand(result, operands[2],operands[3]);
       
  return (unsigned)  result;
}



uint32_t ASMmul(char * operands[]) {
  
  int32_t result = 0;

  //setting A bit 21 to 0
  setBit(&result, 0, 21); 

  //setting S bit 20 to 0
  setBit(&result, 0, 20);

  //setting Cond field to 1110
  result |= uncond_mask;

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
  result |= uncond_mask;

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

int32_t setDataIntrBits(int32_t result,int s,
	       	int opcode, int rn, int rd){
  result |= uncond_mask;
  setBits(&result,21,&opcode,0,4);
  setBit(&result,s,20);
  setBits(&result,16,&rn,0,4);
  setBits(&result,12,&rd,0,4);
//printf("s=%i  opcode=%i  rn=%i  rd=%i\n",s,opcode,rn,rd);
//printf("bla %u\n",result);
  return result;
}

int32_t setOperand(int32_t result, char* str ,char* shift){
 
 int operand2 =0;

 if(*(str) == '#'){
   
   // setting i bit
   setBit(&result, 1 , 25);

   // store operant in intruction bits
   operand2 = getRegBin(str);

   int rotation = 0;
   unsigned int left,right;
   while (abs(operand2)>255){
   // need to rotate
   left = (unsigned) operand2<<2;
   right = (unsigned) operand2>>30;
   operand2 = (unsigned int)(right | left);
   rotation ++;
//   printf("left = %u  right = %u  %u\n",left,right,operand2);
   }
 
//   printf("%u\n",rotation);
   setBits(&result, 8, &rotation, 0, 4);
   setBits(&result, 0, &operand2, 0, 8);
//   printf("%u\n",result);
   return result;
 }
 
 operand2 = getRegBin(str);
 setBits(&result, 0, &operand2, 0, 4);

 if (shift != NULL){
 // shifting by some value
 char type[3];
 strncpy(type,shift,3);
 if(strcmp(type,"lsr")){
  setBit(&result, 0, 5);
  setBit(&result, 1, 6);
 } else if (strcmp(type,"lsl")){
  setBit(&result, 0, 5);
  setBit(&result, 0, 6);
 } else if (strcmp(type,"asr")){
  setBit(&result, 1, 5);
  setBit(&result, 0, 6);
 } else if (strcmp(type,"ror")){
  setBit(&result, 1, 5);
  setBit(&result, 1, 6);
 }

 char reg[3];
 strcpy(reg, &(shift[4]));
 int reg_num = getRegBin(reg);
 setBits(&result, 8, &reg_num, 0, 4);
 setBit(&result, 1 ,4); 
} 
// printf("%u\n",result);
 return result;
}

uint32_t ASMldr(char * operands[]) {
//instruction has syntax ldr Rd, <address>  
//  Rd := (address) 
//"ldr r0, [r1,r2]" will give an array of 
//{"r0", "[r1", "r2"]"}

  //if argument is not greater than 0xFF, it is redirected to mov
  if(!(strncmp(operands[1], "=0xFF", 32) > 0)) {
    return ASMmov(operands);
  }

  int32_t result = 0;

  //setting L bit 20 to 1
  setBit(&result, 1, 20);   

  //setting COND
  int cond = 1110;
  setBits(&result, 28, &cond, 0, 4);

  //setting bits 27-26 to 01
  setBit(&result, 0, 27);   
  setBit(&result, 1, 26);   

  //setting bits 21 22 to 01
  setBit(&result, 0, 21);   
  setBit(&result, 0, 22);   

  //setting bits of rd using operands
  int rd_bin = getRegBin(operands[0]);
  setBits(&result, 12, &rd_bin, 0 , 4);

  //if <address> is a numeric constant of the form: <=expression>
  if(*(operands[1]) == '='){
    
    //setting the base register as the pc register (r15)
    int pc = 15;
    setBits(&result, 16, &pc, 0, 4);

    //obtaining arg as int without the '='
    char* argStr = operands[1];
    argStr++;
    int arg = atoi(argStr);

    //incrementing num_of_lines in the file and extra_data before storing arg
    num_of_lines++;
    extra_data++;
    bin_instr[num_of_lines] = arg;

    //curr has to be amended form current line_num due to PC being off by 
    //8-bytes. Which is 2 instruction lines
    int curr = line_num + 2;

    //calculating offset (in bytes) from where the arg was placed to curr
    int offset = (num_of_lines - curr) * INSTRUCTION_BYTE_SIZE; 

    //setting offset to bits 0-12
    setBits(&result, 0, &offset, 0, 12);
    
    //setting P bit (24th bit) to 1 for pre-indexed
    setBit(&result, 24, 1);


  } else{

    //if <address> is a pre-indexed address specificaition
    //if <address> is a post-indexing addressing specification
    getAddress(&result, operands);

  }

  //bits 20 21 will be 0 as result is initialised

  return (unsigned) result;

}

uint32_t ASMstr(char * operands[]) {
//instruction has syntax str Rd, <address>  
// (address) := Rd

  int32_t result = 0;

  //setting L bit 20 to 0
  setBit(&result, 0, 20);   

  //setting bits 27-26 to 01
  setBit(&result, 0, 27);   
  setBit(&result, 1, 26);   

  //setting bits of rd using operands
  int rd_bin = getRegBin(operands[0]);
  setBits(&result, 12, &rd_bin, 0 , 4);

  //if <address> is a pre-indexed address specificaition
  //if <address> is a post-indexing addressing specification
  getAddress(&result, operands);

  //bits 20 21 will be 0 as result is initialised
  
  return (unsigned) result;

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

