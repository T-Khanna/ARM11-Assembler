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
#include "assemble.c"

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

void getAddress(int *result, char** operands){

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

uint32_t branch_mask = 0x0a000000;

uint32_t compute_cond(enum mnemonic_code cond) {
  cond -= 16;
  cond <<= 28;
  return cond + branch_mask;
}

uint32_t ASMbeq(char * operands[]) {
  return compute_cond(BEQ);
}

uint32_t ASMbne(char * operands[]) {
  return compute_cond(BNE);
}

uint32_t ASMbge(char * operands[]) {
  return compute_cond(BGE);
}

uint32_t ASMblt(char * operands[]) {
  return compute_cond(BLT);
}

uint32_t ASMbgt(char * operands[]) {
  return compute_cond(BGT);
}

uint32_t ASMble(char * operands[]) {
  return compute_cond(BLE);
}

uint32_t ASMb(char * operands[]) {
  return compute_cond(B);
}





