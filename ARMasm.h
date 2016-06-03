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
  int position;

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

uint32_t ASMadd(int32_t operands[]);

uint32_t ASMsub(int32_t operands[]);

uint32_t ASMrsb(int32_t operands[]);

uint32_t ASMand(int32_t operands[]);

uint32_t ASMeor(int32_t operands[]);

uint32_t ASMorr(int32_t operands[]);

uint32_t ASMmov(int32_t operands[]);

uint32_t ASMtst(int32_t operands[]);

uint32_t ASMteq(int32_t operands[]);

uint32_t ASMcmp(int32_t operands[]);

uint32_t ASMmul(int32_t operands[]);

uint32_t ASMmla(int32_t operands[]);

uint32_t ASMldr(int32_t operands[]);

uint32_t ASMstr(int32_t operands[]);

uint32_t ASMbeq(int32_t operands[]);

uint32_t ASMbne(int32_t operands[]);

uint32_t ASMbge(int32_t operands[]);

uint32_t ASMblt(int32_t operands[]);


uint32_t ASMbgt(int32_t operands[]);

uint32_t ASMble(int32_t operands[]);

uint32_t ASMb(int32_t operands[]);

uint32_t ASMlsl(int32_t operands[]);

uint32_t ASMandeq(int32_t operands[]);

#endif
