/////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: assemble_helper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ARMasm.h"
#include "../bitOpers/bitOper.h"

//-- GLOBAL VARIABLES ---------------------------------------------------------

extern int label_count;
extern uint32_t bin_instr_curr[MAX_LINES];
extern int num_of_lines;
extern int line_num;
extern int valid_line_num;
extern int extra_data;
const uint32_t uncond_mask = (1<<31) + (1<<30) + (1<<29);

//-- FUNCTION DECLARATIONS ----------------------------------------------------

int32_t setDataIntrBits(int32_t result,
		int s, int opcode, int rn, int rd);
int32_t setOperand(int32_t result, char* str, char* shift);
int numFromStr(char* operand);
void resPrePostAddressing(int *result, char** operands);

//-- HELPER FUNCTIONS -------------------------------//

/*
    Helper function to Data Processing
    set the given operands to instruction,
		S, opcode, Rn, Rd
*/
int32_t setDataIntrBits(int32_t result,int s,
	       	int opcode, int rn, int rd){

	//setting uncondition condition to instruction
  result |= uncond_mask;

	//setting given values to instruction
  setBits(&result,21,&opcode,0,4);
  setBit(&result,s,20);
  setBits(&result,16,&rn,0,4);
  setBits(&result,12,&rd,0,4);

  return result;
}

/*
   Calculates and sets operand to the given result using
	 the given values
*/

int32_t setOperand(int32_t result, char* str ,char* shift){

   int operand2 = 0;

   // if operand is a immmediate value
   if(*(str) == '#'){

     // setting i bit
     setBit(&result, 1 , 25);

     // store operant in intruction bits
     operand2 = numFromStr(str);

     // if operand is a rotated value
     int rotation = 0;  // store number of rotations
     unsigned int left,right;

     while (abs(operand2)>255){
       // rorating the operand
       left = (unsigned) operand2<<2;
       right = (unsigned) operand2>>30;
       operand2 = (unsigned int)(right | left);
       rotation ++;
     }

    // set number of rotations
    setBits(&result, 8, &rotation, 0, 4);
    // set the operand
    setBits(&result, 0, &operand2, 0, 8);

    return result;
   }

  // operand is a register

  // getting the register number from the string
  operand2 = numFromStr(str);
  //setting register value to bits
  setBits(&result, 0, &operand2, 0, 4);

  // check if regiser are shifted
  if (shift != NULL){

     // getting type of shift
     char type[] = "lsr";
     strncpy(type,shift,3);

     //setting bits according to shift
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

     if (shift[4] == '#'){
        // shift is a expression
        char reg[4];
        strcpy(reg, &(shift[4]));

	      //getting amount of shift
        int reg_num = numFromStr(reg);

	      //setting bits for amount of shift
        setBits(&result, 7, &reg_num, 0, 5);
      	 //setting bit to show the shift expression
        setBit(&result, 0 ,4);
     } else {
        //shift is a register
        char reg[3];
        strcpy(reg, &(shift[4]));

        //getting register number for the shift
        int reg_num = numFromStr(reg);
        // setting the register number to instruction
        setBits(&result, 8, &reg_num, 0, 4);
        // setting bit to show that shifted by register value
        setBit(&result, 1 ,4);
     }
  }

  return result;
}

/*
   Convert string to number
*/

int numFromStr(char* operand){

  //removing the r in the operandister string gived
  operand++;

  //obtaining register number in decimal
  return (int) strtol(operand, NULL, 0);
}

/*

*/

void resPrePostAddressing(int *result, char** operands){
  //"ldr r0, [r1,r2,lsl #2]" will give an array of
  //{"r0", "[r1", "r2", "lsl #2]"}


  //setting base register Rn to bits 16-19 in result
  char* rn_str = operands[0];
  //rn_str++; //ommitting the '[' in "[rn"
  int rn_num = numFromStr(rn_str);
  //printf("rn = %u\n",rn_num);
  setBits(result, 16, &rn_num, 0, 4);
  //printf("setted rn = %u\n",*result);

  //identifying [Rn] from [Rn
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

      ////setting I bit (bit 25)
      //setBit(result, 1, 25);

    } else {

      //two possibilities
      //[Rn], #expression
      //(Opt) [Rn], {+/-}Rm{, <shift>}

      char *hashInOp3 = strchr(operands[2], '#');
      if(hashInOp3 != NULL){
        //set P bit to 1 due to Post-indexing
        setBit(result, 0, 24);

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
        setBit(result, 0, 25);

      } else {

        *result = setOperand(*result, operands[2], operands[3]);

        //setting I bit (bit 25) //TODO
        setBit(result, 1, 25);

      }

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


    char *hashInOp3 = strchr(operands[2], '#');
    if(hashInOp3 != NULL){

        puts("Hello world");
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

      puts(operands[2]);

      if(*(operands[2]) == 'r'){
        //setting I bit (bit 25)
        setBit(result, 1, 25);
      }

    } else {

        //setting I bit (bit 25)
        setBit(result, 1, 25);


        if(operands[3] != NULL){
          *(operands[3] + strlen(operands[3]) - 1) = '\0';
        } else {
          *(operands[2] + strlen(operands[2]) - 1) = '\0';
        }

        *result = setOperand(*result, operands[2], operands[3]);

    }

    operands[1]++;
    int rn_num = numFromStr(operands[1]);
    setBits(result, 16, &rn_num, 0, 4);

  }
}
