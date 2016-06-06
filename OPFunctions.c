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
//extern uint32_t *bin_instr;
extern uint32_t bin_instr_curr[MAX_LINES];
extern int num_of_lines;
extern int line_num;
extern int extra_data;

//-- FUNCTION DECLARATIONS ----------------------------------------------------

int32_t setDataIntrBits(int32_t result,
		int s, int opcode, int rn, int rd);
int32_t setOperand(int32_t result, char* str, char* shift);

// uncondiniotanl mask
const uint32_t uncond_mask = (1<<31) + (1<<30) + (1<<29);

int numFromStr(char* operand){

  //removing the r in the operandister string gived
  operand++;

  //obtaining register number in decimal
  return (int) strtol(operand, NULL, 0);

}

void resPrePostAddressing(int *result, char** operands){
  //"ldr r0, [r1,r2,lsl #2]" will give an array of 
  //{"r0", "[r1", "r2", "lsl #2]"}


  //for(int i = 0; i < 6; i ++){
  //  if(operands[i] != NULL){
  //    puts(operands[i]);
  //  }
  //}
  
  //setting base register Rn to bits 16-19 in result
  char* rn_str = operands[0];
  rn_str++; //ommitting the '[' in "[rn"
  int rn_num = numFromStr(rn_str);
  setBits(result, 16, &rn_num, 0, 4);

  //identifying [Rn] from [Rn
  //char lastCharInOP1 = *(operands[1]) - 1; //TODO: Not sure if this is working
  //int closeBracket = (lastCharInOP1 == ']');
  //returns NULL if not found. index pointer if found
  char *closeBracketInOp0 = strchr(operands[1], ']');

  if(closeBracketInOp0 != NULL) { 
  //if operand0 has a ']' in it
    
    if(operands[2] == NULL) {

      //if there is no second operand set P bit to 1 due to Pre-indexing
      operands[1]++;
      int rn_num = numFromStr(operands[1]);
      setBits(result, 16, &rn_num, 0, 4);
      setBit(result, 1, 24);

    } else {

      //set P bit to 1 due to Post-indexing
      setBit(result, 0, 24);

			//two possibilities
			//[Rn], #expression
			//(Opt) [Rn], {+/-}Rm{, <shift>}

      int32_t offset = numFromStr(operands[2]);

      //checking for sign using sign bit 31
      int isNegative = getBit(&offset, 31);

      //setting bit for U (bit 23) depending on if offset is negative or not
      //determines if offset should be added or subtracted.
      setBit(result, isNegative == 0, 23);

      //amending offset based on sign
      if(isNegative) {
        offset = -offset;
      }
  
      offset = (uint32_t) offset;

      //setting bits 0-11 for offset
      setBits(result, 0, &offset, 0, 11);

      //setting I bit (bit 25)
      setBit(result, 1, 25);

      operands[1]++;
      int rn_num = numFromStr(operands[1]);
      setBits(result, 16, &rn_num, 0, 4);

    }


  } else {
    //if operand0 does not have a ']' in it

    //set P bit to 1 due to Pre-indexing
    setBit(result, 1, 24);

		//One of the following
		//[Rn, #expression]
		//(Opt) [Rn, {+/-}Rm{, <shift>}]
    
    //remove the char ']' from operand2 "#expr]"
    //operands[2] = blah(operands[2]);

    //removing ']' from second operand
    *(operands[2] + strlen(operands[2]) - 1) = '\0';

    int offset = numFromStr(operands[2]);

    //checking if offset fits
    //if(offset < 0xfffff800 || offset > 0x7ff) {
    //  printf("ERROR: Cannot fit offset value \"%d\" to 12 bits in ldr\n"
    //           ,offset);
    //  exit(EXIT_FAILURE);
    //}

    //checking for sign using sign bit 31
    int isNegative = getBit(&offset, 31);

    //setting bit for U (bit 23) depending on if offset is negative or not
    //determines if offset should be added or subtracted.
    setBit(result, isNegative == 0, 23);

    //amending offset based on sign
    if(isNegative) {
      offset = -offset;
    }

    offset = (uint32_t) offset;

    //setting bits 0-11 for offset
    setBits(result, 0, &offset, 0, 12);

    //setting I bit (bit 25)
    setBit(result, 1, 25);

    operands[1]++;
    int rn_num = numFromStr(operands[1]);
    setBits(result, 16, &rn_num, 0, 4);

  }  
}

uint32_t ASMadd(char* operands[]) {
  int32_t result = 0;
  //setting given intruction bits
  result = setDataIntrBits(result, 0, ADD, numFromStr(operands[1]),
      numFromStr(operands[0]));
  
  result = setOperand(result, operands[2], operands[3]);
 // printf("final result = %u\n",result);
  return (unsigned) result;
}

uint32_t ASMsub(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, SUB, numFromStr(operands[1]),
      numFromStr(operands[0]));

  result = setOperand(result, operands[2],operands[3]);

  return (unsigned) result;
}

uint32_t ASMrsb(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, RSB, numFromStr(operands[1]),
      numFromStr(operands[0]));

  result = setOperand(result, operands[2],operands[3]);
  
  return (unsigned) result;
}

uint32_t ASMand(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, AND, numFromStr(operands[1]),
      numFromStr(operands[0]));

  result = setOperand(result, operands[2],operands[3]);

  return (unsigned) result;
}

uint32_t ASMeor(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, EOR, numFromStr(operands[1]),
      numFromStr(operands[0]));
   
  result = setOperand(result, operands[2],operands[3]);
   
  return (unsigned) result;
}

uint32_t ASMorr(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, ORR, numFromStr(operands[1]),
      numFromStr(operands[0]));
 
  result = setOperand(result, operands[2],operands[3]);
       
  return (unsigned) result;
}

uint32_t ASMmov(char* operands[]) {

  // fix this
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 0, MOV, 0, numFromStr(operands[0]));

  result = setOperand(result, operands[1],operands[2]);
       
  return (unsigned) result;
}

uint32_t ASMtst(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, TST, numFromStr(operands[0]),0);

  result = setOperand(result, operands[1],operands[2]);
       
  return (unsigned) result;
}

uint32_t ASMteq(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, TEQ, numFromStr(operands[0]),0);

  result = setOperand(result, operands[1],operands[2]);

  return (unsigned) result;
}

uint32_t ASMcmp(char* operands[]) {
  int32_t result = 0;
   //setting given intruction bits
  result = setDataIntrBits(result, 1, CMP, numFromStr(operands[0]),0);
//    printf("%u\n",result);
 
  result = setOperand(result, operands[1],operands[2]);
       
  return (unsigned) result;
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
  int rd_num = numFromStr(operands[0]);
  setBits(&result, 16, &rd_num, 0, 4);
  
  //setting bits of rm using operands
  int rm_num = numFromStr(operands[1]);
  setBits(&result, 0, &rm_num, 0, 4);

  //setting bits of rs using operands
  int rs_num = numFromStr(operands[2]);
  setBits(&result, 8, &rs_num, 0 , 4);

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
  int rd_num = numFromStr(operands[0]);
  setBits(&result, 16, &rd_num, 0, 4);
  
  //setting bits of rm using operands
  int rm_num = numFromStr(operands[1]);
  setBits(&result, 0, &rm_num, 0, 4);

  //setting bits of rs using operands
  int rs_num = numFromStr(operands[2]);
  setBits(&result, 8, &rs_num, 0 , 4);

  //setting bits of rn using operands
  int rn_num = numFromStr(operands[3]);
  setBits(&result, 12, &rn_num, 0, 4);

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
   operand2 = numFromStr(str);

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
 
 operand2 = numFromStr(str);
 setBits(&result, 0, &operand2, 0, 4);

 //printf("before shift type %u\n",result);

 if (shift != NULL){
 // shifting by some value
 char type[3];
 strncpy(type,shift,3);
 if(!strcmp(type,"lsr")){
  setBit(&result, 1, 5);
  setBit(&result, 0, 6);
 } else if (!strcmp(type,"lsl")){
  setBit(&result, 0, 5);
  setBit(&result, 0, 6);
 } else if (!strcmp(type,"asr")){
  setBit(&result, 0, 5);
  setBit(&result, 1, 6);
 } else if (!strcmp(type,"ror")){
  setBit(&result, 1, 5);
  setBit(&result, 1, 6);
 }

 //printf(" shifted %u\n",result);

 if (shift[4] == '#'){
  // shift is a expression
  char reg[4];
  strcpy(reg, &(shift[4]));
  int reg_num = numFromStr(reg);
  setBits(&result, 7, &reg_num, 0, 5);  
  setBit(&result, 0 ,4);
 }
 else {
 //shift is a register
 char reg[3];
 strcpy(reg, &(shift[4]));
 int reg_num = numFromStr(reg);
 setBits(&result, 8, &reg_num, 0, 4);
 setBit(&result, 1 ,4); 
 }
} 

//printf("%u\n",result);
 return result;
}

uint32_t ASMldr(char * operands[]) {
//instruction has syntax ldr Rd, <address>  
//  Rd := (address) 
//"ldr r0, [r1,r2]" will give an array of 
//{"r0", "[r1", "r2"]"}



  //if argument is not greater than 0xFF(255 in dec), it is redirected to mov
  if(!(numFromStr(operands[1]) > 255) && (*(operands[1])) != '[') {

    //TODO: testing
    puts("going to mov");

    *(operands[1]) = '#';

    return ASMmov(operands);
  }

  //settting all the known bits of the result
  int32_t result = 0;
	result = 0xe4900000;

  //setting bits of rd using operands
  int rd_num = numFromStr(operands[0]);
  setBits(&result, 12, &rd_num, 0 , 4);

  //if <address> is a numeric constant of the form: <=expression>
  if(*(operands[1]) == '='){
    
    //setting the base register as the pc register (r15)
    int pc = 15;
    setBits(&result, 16, &pc, 0, 4);

    //obtaining arg as int without the '=' using numFromStr which omits the
    //first character and returns the rest of the operand as an int
    //int arg = numFromStr(operands[1]);
    uint32_t arg = numFromStr(operands[1]);
    //char* argStr = operands[1];
    //argStr++;
    //int arg = atoi(argStr);

    //incrementing num_of_lines in the file and extra_data before storing arg
    num_of_lines++;
    extra_data++;
    bin_instr_curr[num_of_lines - 1] = arg;

    //curr has to be amended form current line_num due to PC being off by 
    //8-bytes. Which is 2 instruction lines
    int curr = line_num + 2;

    //calculating offset (in bytes) from where the arg was placed to curr
    int offset = (num_of_lines - curr) * INSTRUCTION_BYTE_SIZE; 

    //setting offset to bits 0-12
    setBits(&result, 0, &offset, 0, 12);
    
    //setting P bit (24th bit) to 1 for pre-indexed
    setBit(&result, 1, 24);


  } else{

    //if <address> is a pre-indexed address specificaition
    //if <address> is a post-indexing addressing specification
    resPrePostAddressing(&result, operands);

  }

  //bits 20 21 will be 0 as result is initialised

  return (unsigned) result;

}

uint32_t ASMstr(char * operands[]) {
//instruction has syntax str Rd, <address>  
// (address) := Rd
// str r1,[r2],r4

  //setting all known bits in register
  int32_t result = 0;
  result = 0xe4800000;

  //setting bits of rd using operands
  int rd_num = numFromStr(operands[0]);
  setBits(&result, 12, &rd_num, 0 , 4);

  //if <address> is a pre-indexed address specificaition
  //if <address> is a post-indexing addressing specification
  resPrePostAddressing(&result, operands);

  return (unsigned) result;

}

uint32_t ASMlsl(char * operands[]) {
//lsl Rn, <#expression> as though it were 
//mov Rn, Rn, lsl <#expression>

//Your assembler should compile the instruction lsl Rn, <#expression> as though 
//it were mov Rn, Rn, lsl <#expression>. The binary form of this can be found 
//in the emulator section, under Data Processing Instructions, when Operand2 is 
//a register, shifted by a constant amount.  

  //finding out suitable operands to call mov from operands specified
  puts(operands[1]);
  puts(operands[0]);
  char lsl[12]; // how big can this be?
  strcpy(lsl, "lsl ");
  char* lsl_expr = strcat(lsl, operands[1]);
  char* operands_for_mov[4] = {operands[0], operands[0], lsl_expr};

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

int32_t calculate_branch(enum mnemonic_code cond, char* operand) {
  cond -= 16;
  int32_t offset = atoi(operand);
//  offset >>= 2;
//  offset &= 0x00ffffff;
  return (cond << 28) | 0x0a000000 | ((offset >> 2) & 0x00ffffff);
}

uint32_t ASMbeq(char * operands[]) {
  return calculate_branch(BEQ, operands[0]);
}

uint32_t ASMbne(char * operands[]) {
  return calculate_branch(BNE, operands[0]);
}

uint32_t ASMbge(char * operands[]) {
  return calculate_branch(BGE, operands[0]);
}

uint32_t ASMblt(char * operands[]) {
  return calculate_branch(BLT, operands[0]);
}

uint32_t ASMbgt(char * operands[]) {
  return calculate_branch(BGT, operands[0]);
}

uint32_t ASMble(char * operands[]) {
  return calculate_branch(BLE, operands[0]);
}

uint32_t ASMb(char * operands[]) {
  return calculate_branch(B, operands[0]);
}

