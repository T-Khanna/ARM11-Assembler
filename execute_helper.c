///////////////////////////////////////////////////////////////////////////////
//ARM Group Project - Year 1 (Group 40)
//_____________________________________________________________________________
//
//File: execute_helper.c
//Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdio.h>
#include "ARMgen.h"
#include "bitOper.h"
#include "execute_helper.h"

// ------------------------- HELPER FUNCTIONS ------------------------------ //

int get_operand2(int32_t* instr,current_state* curr_state, int i);
int getImmVal(int32_t* instr);
int getRegVal(int32_t* instr,current_state* curr_state);
int32_t readMemory(int mem_address, current_state* curr_state);
void writeMemory(int mem_address,int source,current_state* curr_state); 

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
                  curr_state->registers[rn] >= op2,29);
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
    return getRegVal(instr, curr_state);
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
 
  /*
    Help to load and store from and to memory
    Imm offset:     i = bit 25
    pre/post index  p = bit 24
    up bit          u = bit 23
    load/store bit  l = bit 20
    Base reg        rn = bit 16 to bit 19
    source/dest reg rd = bit 12 to bit 15
    offset             = bit 0 to bit 11
  */
void single_data_transfer(int32_t* instr, current_state* curr_state){
    int i = getBit(instr, 25);
    int p = getBit(instr, 24);
    int u = getBit(instr, 23);
    int l = getBit(instr, 20);
    int rn = getBits(instr, 16, 4);
    int rd = getBits(instr, 12, 4);
    int offset = 0;

    if(i){
       //make temperary instruction
       int temp = *instr;
       // settting temperary to 0 to avoid any CPSR setting
       setBit(&temp,20,0);
      
       offset = getRegVal(&temp,curr_state);
    } else {
       offset = getBits(instr, 0,12);
    }

    int mem_address = curr_state->registers[rn];

//    printf("%i\n",mem_address);

    // if pc is rn 
    if (rn == 15){
    // making byte addressable and adjusting for pipeline
    mem_address = mem_address * 4 + 4;
    }

    if (p) {
        // value of base register not changed 
        (u) ? (mem_address += offset) : (mem_address -= offset);
        
    } else {
       // updating value of rn
       (u) ? (curr_state->registers[rn] += offset) : 
                (curr_state->registers[rn] -= offset); 
    }

//   printf("%i\n",mem_address);

    if (mem_address >= 65536){
       printf("Error: Out of bounds memory access at address 0x%08x\n", mem_address);
       return;
    }

    if (l) {
      // load
//     printf("%i\n",mem_address);
      curr_state->registers[rd] = readMemory(mem_address,curr_state);
    } else {
      //store
      for (int i = 0; i<4 ;i++){
//    printf("%i\n",rd);
      writeMemory(mem_address,curr_state->registers[rd], curr_state);
      }
    }
}

int32_t readMemory(int mem_address, current_state* curr_state){

  int index = mem_address % 4;
  int32_t result = 0;
   
  setBits(&result,0,&curr_state->memory[mem_address/4]
          ,8*index,(4-index)*8);

  if (index == 0){return result;}  

  if(mem_address/4 == MEMORY_CAPACITY -1){
       return result;
    } else {
     setBits(&result,(4-index)*8,&curr_state->memory[mem_address/4 + 1]
             ,0,index * 8);
     return result;
 }
} 

void writeMemory(int mem_address,int source,current_state* curr_state){

   int index = mem_address % 4;

//   printf("%i\n",source);   

   setBits(&(curr_state->memory[mem_address/4]),8*index,
           &source,0,(4-index)*8);
 
   if (index == 0){return;}

   if(mem_address/4 == MEMORY_CAPACITY -1){
       return;
    } else {
   setBits(&(curr_state->memory[mem_address/4 + 1]),0,
           &source,(4-index),index*8); 
   }
}


// ------------------------- Multiply -----------------------------------------
  
 /* 
   Multiply and store value in a register according to given 
   given values in instruction
   Accumulator:   a = bit 21
   Set condition: s = bit 20
   Destination    rd = bit 16 to 19
   Operator reg   rn = bit 12 to 15 
                  rs = bit 8 to 11
                  rm = bit 0 to 3 
 */
void multiply(int32_t* instr, current_state* curr_state){
   // getting variables we know

   int a = getBit(instr, 21);
   int s = getBit(instr, 20);
   int rd = getBits(instr, 16, 4);
   int rn = getBits(instr, 12, 4);
   int rs = getBits(instr, 8, 4);
   int rm = getBits(instr, 0, 4);

   if (a){
      //rd = rm * rs + rn
      curr_state->registers[rd] = 
         curr_state->registers[rm] * curr_state->registers[rs]
              + curr_state->registers[rn];
   } else {
      //rd = rm * rs
      curr_state->registers[rd] = 
         curr_state->registers[rm] * curr_state->registers[rs];
   }

   // setting flags
   if (s){
      // setting flag n
      setBit(&(curr_state->CPSR),
                   getBit(&(curr_state->registers[rd]),31),31);
      // setting flag z
       setBit(&(curr_state->CPSR),
                   curr_state->registers[rd]==0,30);
   }
}
