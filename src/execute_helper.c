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

int get_operand2(int32_t* instr,current_state* curr_state, int i);
int getImmVal(int32_t* instr);
int getRegVal(int32_t* instr,current_state* curr_state);

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
  
  /*
     Process given instruction and perform various functions
     I = bit 25
     opcode = 21 to 24
     S = bit 20
     Rn = bits 16 to 19
     Rd = bits 12 to 15
     op2 = bits 0 to 11
  */

void data_processing(int32_t* instr, current_state* curr_state){
     
     //geting variables we know
     int i = getBit(instr, 25);
     int opcode = getBits(instr, 21, 4);
     int s = getBit(instr, 20);
     int rn = getBits(instr, 16, 4); 
     int rd = getBits(instr, 12, 4);
     int op2 = get_operand2(instr,curr_state,i);

     // temp variable
     int result = 0;

     switch(opcode){
        
        // and rd = rn & op2
        case 0: curr_state->registers[rd] 
                 = curr_state->registers[rn] & op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);
                }
                break;
        // eor rd = rn ^ op2
        case 1: curr_state->registers[rd] 
                 = curr_state->registers[rn] ^ op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);
                }
                break;
        // sub : rd = rn - op2
        case 2: curr_state->registers[rd] 
                 = curr_state->registers[rn] - op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);

                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd] >= op2,29);
                }
                break;
        // rsb : rd = op2 - rn
        case 3: curr_state->registers[rd]
                 = op2 - curr_state->registers[rn];
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);

                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd] <= op2,29);
                }
                break; 
        // add : rd = rn + op2 
        case 4: curr_state->registers[rd]
                 = curr_state->registers[rn] + op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);

                  int n1 = getBit(&curr_state->registers[rn],31);
                  int n2 = getBit(&op2,31); 
                  int r  = getBit(&curr_state->registers[rd],31);
 
                  setBit((&curr_state->CPSR),
                  (!n1 & !n2 & r)|(n1 & n2 & !r),29);
                }
                break;
        // tst : and cpsr bits stored
        case 8: result
                 = curr_state->registers[rn] & op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  result==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&result,31),31);
                }
                break;
        // teq : eor cpsr bit stored
        case 9: result
                 = curr_state->registers[rn] ^ op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  result==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&result,31),31);
                }
                break;
        // cmp : sub cpsr bit stored
        case 10: result
                 = curr_state->registers[rn] - op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  result==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&result,31),31);

                  setBit((&curr_state->CPSR),
                  result >= op2,29);
                }
                break; 
        // orr : rd = rn | op2
        case 12: curr_state->registers[rd]
                 = curr_state->registers[rn] | op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);
                }
                break;
        // mov : rd = op2
        case 13: curr_state->registers[rd]
                 = op2;
                if(s){
                  setBit((&curr_state->CPSR),
                  curr_state->registers[rd]==0,30);

                  setBit((&curr_state->CPSR),
                  getBit(&curr_state->registers[rd],31),31);
                }
                break;
        }
} 

//helper function to calculate operand2 
int get_operand2(int32_t* instr, current_state* curr_state,int i){

    if (i == 1){
       return getImmVal(instr);
    }
    return  getRegVal(instr, curr_state);
}

//------------------------------
int getImmVal(int32_t* instr){

   // get value 
   int value = getBits(instr, 0, 8);
    
   //get rotations
   int rot_num = getBits(instr, 8, 4) * 2;

   return rotate_right(value,(unsigned int)rot_num);
}

//-----------------------------
int getRegVal(int32_t* instr, current_state* curr_state){
   
   int result = 0;

   // get register 
   int rm = getBits(instr, 0, 4);

   //get the value of register
   int value = curr_state->registers[rm];

   //get shift amount
   unsigned int amount = 0;

   if (getBit(instr,4) == 0){
      // amount is given by integer
      amount = (unsigned int)(getBits(instr,7,5));
   } else {
      // amount given by last byte of rs
      int rs = getBits(instr, 8, 4); 
      amount = (unsigned int)(curr_state->registers[rs] & 0xff);
   }
   
   int select_shift = getBits(instr, 5, 2);

   int flag_value = 0;
   switch(select_shift){

     case 0: result = value << amount;
             if(getBit(instr,20)){
                 if (amount != 0){
                 getBit(&value,32-amount);}
                 setBit(&(curr_state->CPSR),flag_value,29);
             }
             return result;

     case 1: result = (unsigned int)value >> (amount);
             break;

             // for preserving first int shift without (unsigned) 
     case 2: result = value >> amount;
             break;
    
     case 3: result = rotate_right(value,amount);
             break;
  }
 
  if(getBit(instr,20)){
                 if (amount != 0){   
                 getBit(&value,amount-1);}
                 setBit(&(curr_state->CPSR),flag_value,29);
             }

  return result;
}
// ------------------------- Single Data Transfer -----------------------------
  
  // Insert single data transfer helper functions here
void multiply(int32_t* instr, current_state* curr_state){

}
// ------------------------- Multiply -----------------------------------------
  
  // Insert multiply helper functions here
void single_data_transfer(int32_t* instr, current_state* curr_state){

}
