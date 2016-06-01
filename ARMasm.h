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
  int code;
  int32_t operands[OPERAND_SIZE];

} tokenised;

struct mnemonic_code_mapping {
  char *mnemonic; 
  int code;
};

struct mnemonic_code_mapping op_table[] = {
  
  // Data Processing
  {"add", 4},
  {"sub", 2},
  {"rsb", 3},
  {"and", 0},
  {"eor", 1},
  {"orr", 12},
  {"mov", 13},
  {"tst", 8},
  {"teq", 9},
  {"cmp", 10},

  // Multiply
  {"mul", 14},
  {"mla", 15},

  // Single Data Transfer
  {"ldr", 5},
  {"str", 6},

  // Branch 
  {"beq", 16}, 
  {"bne", 17}, 
  {"bge", 26}, 
  {"blt", 27}, 
  {"bgt", 28}, 
  {"ble", 29}, 
  {"b", 30},  

  // Special
  {"lsl", 31},
  {"andeq", 32}

};


#endif
