////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: ARMgen.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//------------------------ ARM GENERAL DEFINITIONS -----------------------------
// Contains the definitions and structure for the Rasberry Pi system.

#ifndef _ARM_GEN
  #define _ARM_GEN

#include <stdint.h>



//-- CONSTANTS -----------------------------------------------------------------

#define MEMORY_CAPACITY (65536/4) //memory = 65536 bytes/ 4 words 
#define TOTAL_REGISTERS 17
#define GEN_PURPOSE_REG 13
#define INSTRUCTION_BYTE_SIZE 4
#define BYTE_SIZE 8 
#define PC registers[15] 
#define CPSR registers[16]


//--  PIPELINE -----------------------------------------------------------------

typedef struct pipeline {
  int32_t fetched;
  int32_t decoded;
} pipeline;


//-- CURRENT STATE -------------------------------------------------------------

typedef struct current_state {
  pipeline pipeline;
  int32_t registers[TOTAL_REGISTERS];
  int32_t memory[MEMORY_CAPACITY];
} current_state;


//-- INSTRUCTION TYPE ----------------------------------------------------------

enum instruction_type {
  Data_processing,
  Multiply,
  Single_data_transfer,
  Branch 
};

#endif

