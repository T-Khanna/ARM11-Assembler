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

//-- ENUM CODE MAPPING ---------------------------------------------------------

enum mnemonic_code {

  AND,
  EOR,
  SUB,
  RSB,
  ADD,
  LDR,
  STR,
  TST = 8,
  TEQ,
  CMP,
  ORR = 12,
  MOV,
  MUL,
  MLA,
  BEQ,
  BNE,
  LSL,
  ANDEQ,
  BGE = 26,
  BLT,
  BGT,
  BLE,
  B,
   
};

//-- SYMBOL TABLE --------------------------------------------------------------

struct symbol_table {

  char* label;
  char** memory_address;

};


//-- TOKENISED -----------------------------------------------------------------

typedef struct tokenised {

  char* label;
  int32_t (*func_pointer)(int32_t[]);
  int32_t operands[OPERAND_SIZE];

} tokenised;

typedef struct mnemonic_code_mapping {
  char *mnemonic; 
  enum mnemonic_code opcode;
  //int32_t (*op_func_pointer)(int32_t[]);
} mnemonic_code_mapping;

//-- GLOBAL VARIABLES ----------------------------------------------------------

struct symbol_table symb_table[MAX_LABELS];

//-- OPFUNCTION DECLARATIONS ---------------------------------------------------

uint32_t ASMadd(uint32_t operands[]);

uint32_t ASMsub(uint32_t operands[]);

uint32_t ASMrsb(uint32_t operands[]);

uint32_t ASMand(uint32_t operands[]);

uint32_t ASMeor(uint32_t operands[]);

uint32_t ASMorr(uint32_t operands[]);

uint32_t ASMmov(uint32_t operands[]);

uint32_t ASMtst(uint32_t operands[]);

uint32_t ASMteq(uint32_t operands[]);

uint32_t ASMcmp(uint32_t operands[]);

uint32_t ASMmul(uint32_t operands[]);

uint32_t ASMmla(uint32_t operands[]);

uint32_t ASMldr(uint32_t operands[]);

uint32_t ASMstr(uint32_t operands[]);

uint32_t ASMbeq(uint32_t operands[]);

uint32_t ASMbne(uint32_t operands[]);

uint32_t ASMbge(uint32_t operands[]);

uint32_t ASMblt(uint32_t operands[]);


uint32_t ASMbgt(uint32_t operands[]);

uint32_t ASMble(uint32_t operands[]);

uint32_t ASMb(uint32_t operands[]);

uint32_t ASMlsl(uint32_t operands[]);

uint32_t ASMandeq(uint32_t operands[]);

#endif
