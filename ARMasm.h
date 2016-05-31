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

#define LIMIT_PER_LINE 511
#define MEMORY_CAPACITY (65536/4)
#define INSTRUCTION_BYTE_SIZE 4
#define BYTE_SIZE 8
#define TOKEN_LIMIT 5


//-- OPCODE --------------------------------------------------------------------

enum opcode {
  
  ADD,
  SUB,
  RSB,
  AND,
  EOR,
  ORR,
  MOV,
  TST,
  TEQ,
  CMP,
  MUL,
  MLA,
  LDR,
  STR,
  BEQ,
  BNE,
  BGE,
  BLT,
  BGT,
  BLE,
  B,
  LSL,
  ANDEQ

};


//-- SYMBOL TABLE --------------------------------------------------------------

struct symbol_table {

  char* *label;
  int32_t* memory_address;

};


//-- TOKENISED -----------------------------------------------------------------

typedef struct tokenised {

  char* label;
  enum opcode op;
  int32_t operands[4];

} tokenised;


#endif
