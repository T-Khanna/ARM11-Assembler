///////////////////////////////////////////////////////////////////////////////
//ARM Group Project - Year 1 (Group 40)
//_____________________________________________________________________________
//
//File: execute_helper.c
//Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "ARMgen.h"
#include "bitOper.h"
#include "execute_helper.h"

// ------------------------- HELPER FUNCTIONS ------------------------------ //


// ------------------------- Branch  ------------------------------------------

  /* 
     Jumps to required branch (pc) of the code
     offset = bit 0 to 23
     offsetn is shifted left 2 and extended to 32 bit 
     and added back to PC
  */
void branch(int32_t* instr, current_state* curr_state){
 
   // get the offset
   int offset = getBits(instr,0,23);
   
   // shift it by 2 
   offset = offset << 2;

   //if signed bits the signed extend offset
   if (getBit(instr,23)){
      offset = offset | (0xFF000000);
   }

   //adding affset to pc
   curr_state->PC = curr_state->PC + (offset/ INSTRUCTION_BYTE_SIZE);
   
   //setting pc for pipeline structure
   curr_state->PC += 1;

}
// ------------------------- Data Processing ----------------------------------
  
  // Insert data processing helper functions here
void data_processing(int32_t* instr, current_state* curr_state){

}
// ------------------------- Single Data Transfer -----------------------------
  
  // Insert single data transfer helper functions here
void multiply(int32_t* instr, current_state* curr_state){

}
// ------------------------- Multiply -----------------------------------------
  
  // Insert multiply helper functions here
void single_data_transfer(int32_t* instr, current_state* curr_state){

}
