///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: bitOper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////


#include "bitOper.h"
#include "ARMgen.h"
#include <stdlib.h>
#include <assert.h>


//-- CONSTANTS ----------------------------------------------------------------

#define WORD_SIZE 32


// get value of multiple bits of given size and position
int getBits(int* num, int index, int size) {
   assert((index >= 0) && (index < WORD_SIZE) &&
          (size >= 0) && (size < WORD_SIZE));
   
   // using result variable as a mask
   int result = 0;
   for (int i = 0; i < size; i++){
      result = result | (1 << (index + i));
   } 
  
   // saving result in result variable
   result = result & (*num);
   return ((unsigned int) result >> index);
}


// set the bits from a given binary number at given positions and of given size
void setBits(int* target, int index_target, 
             int* source, int index_source, int size) {

   // get bits that need to be set from source
   int mask = 0;
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

/*
int convBinToDec(int32_t bin[], int32_t size) {
// converts binary array to int32_t

  int result = 0;
  //int32_t temp = 0;
  for (int32_t i = 0; i < size; i++) {
    result = result * 2 + bin[i];
  }

  //result = temp;
  return result;
}

int32_t* convDecToBin(int32_t num, int32_t size){
// convert int32_t to binary array
  static int32_t result[32];
  for (int i =0; i < size; i++){
    result[size - i - 1] = (num % 2);
    num = num / 2;
  }
  return result;
}

void rotate_right(int32_t array[], int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {
    int temp;
    temp = array[size - 1];
    for (int i = size - 1; i > 0; i--) {
      array[i] = array[i - 1];
    }
    array[0] = temp;
  }
}

void shift_left(int32_t array[], int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {

    updateCarry(array[0]);

    for (int i = size - 1; i > 0; i--) {
      array[i] = array[i - 1];
    }
    array[0] = 0;
  }

}

void shift_right(int32_t array[], int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {

    updateCarry(array[0]);

    for (int i = size - 2; i >= 0; i--) {
      array[i] = array[i + 1];
    }
    array[size - 1] = 0;
  }
}

void arith_shift_right(int32_t array[], int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {
    updateCarry(array[0]);

    for (int i = size - 3; i >= 0; i--) {
      array[i] = array[i + 1];
    }
    array[size - 2] = 0;
  }
}


int * binary_sub(int32_t * arr1, int32_t * arr2, int size) {
  int carry = 0;
  int * resArray = malloc(size * sizeof(int32_t));
    for (int i = 0; i < size; i++) {
      *(resArray + i) = *(arr1 + i) - *(arr2 + i) - carry;
      if (*(resArray + i) < 0) {
        *(resArray) += 2;
        carry = 1;
      } else{
        carry = 0;
      }
    }  
  updateCarry(carry);
  return resArray;
}

int * binary_add(int32_t * arr1, int32_t * arr2, int size) {
  int carry = 0;
  int * resArray = malloc(size * sizeof(int32_t));
    for (int i = 0; i < size; i++) {
      *(resArray + i) = *(arr1 + i) + *(arr2 + i) + carry;
      if (*(resArray + i) > 1) {
        *(resArray) -= 2;
        carry = 1;
      } else{
        carry = 0;
      }
    }
  updateCarry(carry);
  return resArray;
  //free resArray
}

*/

