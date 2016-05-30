///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: ARMasm.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#ifndef _ARM_ASM
#define _ARM_ASM

#include <stdint.h>

//-- CONSTANTS ----------------------------------------------------------------

#define LIMIT_PER_LINE 511

//-- SYMBOL TABLE -------------------------------------------------------------

typedef struct Symbol_table {
  char* *label;
  int32_t *memory_address;
} Symbol_table;


#endif
