///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: bitOper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////



//-------------------------- BITWISE OPERATIONS -------------------------------
// Contains the helper functions that perform bitwise operations on the 
// instruction.


#include "bitOper.h"
#include "ARMgen.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


//-- CONSTANTS ----------------------------------------------------------------

#define WORD_SIZE 32



//gets values of multiple bits of given size and position using pointer
int getBits(int* num, int index, int size) {
   assert((index >= 0) && (index < WORD_SIZE) &&
          (size > 0) && (size <= WORD_SIZE));
   
   //result variable is the mask
   int result = 0;
   for (int i = 0; i < size; i++){
     result = result | (1 << (index + i));
   } 
  
   //saving result in result variable after obtaining bits
   result = result & (*num);
   return ((unsigned int) result >> index);
}


// set the bits from a given binary number at given positions and of given size
void setBits(int* target, int index_target, 
             int* source, int index_source, int size) {

   // get bits that need to be set from source
   int mask = 0;
//   printf("%i %i \n",index_source,size);
   int source_bits = getBits(source, index_source, size);
   source_bits = (source_bits << index_target);   
   for (int i = index_target; i < index_target+size; i++) {
      mask = mask | (1 << i);
   }

   // make the required bit positions 0 by using mask
   *target = *target & (~mask);
   
   // updating the target bits
   *target = (*target | source_bits);

}


// return the bit value of a binary number
int getBit(int* num, int index) {
    assert((index >= 0) && (index < WORD_SIZE));

    int maskedBit = 1 << index;
    return (int32_t)(((unsigned int)(*num & maskedBit)) >> index);

}



// Set the bit value at given index of a given binary number pointer
// using bitoperations
void setBit(int* num, int givenBit, int index) {
   assert((index >= 0) && (index < WORD_SIZE) &&
           (givenBit == 0 || givenBit == 1));

   // first making target int 0, using two's complement then using or to put 
   // given bit there instead
   int twoComp = ~(1 << index);
   *num = ((*num & twoComp) | (givenBit << index));

}

int rotate_right(int num, unsigned int rot_num){
   unsigned int right = (unsigned int)num >> rot_num;   
   unsigned int left = (unsigned int)num << (32 - rot_num);
   return (int)(right | left);
}

