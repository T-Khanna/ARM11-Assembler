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

extern struct mnemonic_code_mapping op_table[];

//-- OPFUNCTION DECLARATIONS --------------------------------------------------

int32_t ARMadd(int32_t operands[]);

int32_t ARMsub(int32_t operands[]);

int32_t ARMrsb(int32_t operands[]);

int32_t ARMand(int32_t operands[]);

int32_t ARMeor(int32_t operands[]);

int32_t ARMorr(int32_t operands[]);

int32_t ARMmov(int32_t operands[]);

int32_t ARMtst(int32_t operands[]);

int32_t ARMteq(int32_t operands[]);

int32_t ARMcmp(int32_t operands[]);

int32_t ARMmul(int32_t operands[]);

int32_t ARMmla(int32_t operands[]);

int32_t ARMldr(int32_t operands[]);

int32_t ARMstr(int32_t operands[]);

int32_t ARMbeq(int32_t operands[]);

int32_t ARMbne(int32_t operands[]);

int32_t ARMbge(int32_t operands[]);

int32_t ARMblt(int32_t operands[]);


int32_t ARMbgt(int32_t operands[]);

int32_t ARMble(int32_t operands[]);

int32_t ARMb(int32_t operands[]);

int32_t ARMlsl(int32_t operands[]);

int32_t ARMandeq(int32_t operands[]);

#endif
