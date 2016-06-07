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
#define TOKEN_LIMIT 10
#define MEMORY_CAPACITY (65536/4)
#define INSTRUCTION_BYTE_SIZE 4
#define INSTRUCTION_BIT_SIZE 32
#define BYTE_SIZE 8
#define OPERAND_SIZE 4
#define PIPELINE_OFFSET 8

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

  uint32_t (*func_pointer)(char* operands[]);
  char* operands[OPERAND_SIZE];

} tokenised;

typedef struct mnemonic_code_mapping {
  char *mnemonic; 
  enum mnemonic_code opcode;
} mnemonic_code_mapping;

//-- GLOBAL VARIABLES ----------------------------------------------------------

struct symbol_table symb_table[MAX_LABELS];

//-- OPFUNCTION DECLARATIONS ---------------------------------------------------

uint32_t ASMadd(char* operands[]);

uint32_t ASMsub(char* operands[]);

uint32_t ASMrsb(char* operands[]);

uint32_t ASMand(char* operands[]);

uint32_t ASMeor(char* operands[]);

uint32_t ASMorr(char* operands[]);

uint32_t ASMmov(char* operands[]);

uint32_t ASMtst(char* operands[]);

uint32_t ASMteq(char* operands[]);

uint32_t ASMcmp(char* operands[]);

uint32_t ASMmul(char* operands[]);

uint32_t ASMmla(char* operands[]);

uint32_t ASMldr(char* operands[]);

uint32_t ASMstr(char* operands[]);

uint32_t ASMbeq(char* operands[]);

uint32_t ASMbne(char* operands[]);

uint32_t ASMbge(char* operands[]);

uint32_t ASMblt(char* operands[]);


uint32_t ASMbgt(char* operands[]);

uint32_t ASMble(char* operands[]);

uint32_t ASMb(char* operands[]);

uint32_t ASMlsl(char* operands[]);

uint32_t ASMandeq(char* operands[]);

#endif
