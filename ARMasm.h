////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: ARMasm.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////

#ifndef _ARM_ASM
#define _ARM_ASM

#include <stdint.h>

//-- CONSTANTS -----------------------------------------------------------------

#define MAX_LABELS 50
#define CHAR_LIMIT 511
#define MAX_LINES 1000
#define TOKEN_LIMIT 5
#define MEMORY_CAPACITY (65536/4)
#define INSTRUCTION_BYTE_SIZE 4
#define INSTRUCTION_BIT_SIZE 32
#define BYTE_SIZE 8
#define OPERAND_SIZE 4

//-- SYMBOL TABLE --------------------------------------------------------------

struct symbol_table {

  char* label;
  char** memory_address;

};


//-- TOKENISED -----------------------------------------------------------------

typedef struct tokenised {

  char* label;
  int8_t op;
  int32_t operands[OPERAND_SIZE];

} tokenised;


#endif
