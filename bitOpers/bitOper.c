////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: bitOper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//-------------------------- BITWISE OPERATIONS --------------------------------
// Contains the helper functions that perform bitwise operations on the
// instruction.

#include "bitOper.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>



//-- CONSTANTS -----------------------------------------------------------------

#define WORD_SIZE 32


//-- FUNCTION DEFINTIONS -------------------------------------------------------

//gets values of multiple bits of given size and position from right using
//the specified pointer. Returns the bits needed to the right of the int.
//Unwanted bits are 0 when returned
int getBits(int* num, int index, int size) {
  assert((index >= 0) && (index < WORD_SIZE) &&
      (size > 0) && (size <= WORD_SIZE));

  //result variable is the mask
  int result = 0;
  for(int i = 0; i < size; i++){
    result = result | (1 << (index + i)); //SC
  }

  //saving result in result variable after obtaining bits
  result = result & (*num);
  return ((unsigned int) result >> index);
}


//sets the bits from a given binary number at given positions and of given size
void setBits(int* target, int index_target,
             int* source, int index_source, int size) {

  //getting bits that need to be set from source in the form of a mask
  int mask = 0;
  int source_bits = getBits(source, index_source, size);
  source_bits = source_bits << index_target; //SC
  for(int i = index_target; i < index_target + size; i++){
    mask = mask | (1 << i);
  }

  //setting the target bits that will be replaced to 0 by inverting the bits
  //of mask.
  *target = *target & (~mask);

  //setting the target bits to source bits using | on 0s that we set earlier
  *target = (*target | source_bits);
}


//returns the bit value of a binary number
int getBit(int* num, int index) {
  assert((index >= 0) && (index < WORD_SIZE));

  int maskedBit = 1 << index;
  int bit = ((unsigned int) (*num & maskedBit)) >> index;
  return (int32_t) bit;
}


// Set the bit value at given index of a given binary number pointer
// using bitoperations
void setBit(int* num, int givenBit, int index) {
  assert((index >= 0) && (index < WORD_SIZE) &&
      (givenBit == 0 || givenBit == 1));

  // first making target int 0, by inverting using two's complement then
  // using | to replace old bit at index by givenBit
  int twoComp = ~(1 << index);
  *num = ((*num & twoComp) | (givenBit << index));
}

//rotates given num rot_num times to the right
int rotateRight(int num, unsigned int rot_num) {

  //shift right rot_num times
  unsigned int right = (unsigned int)num >> rot_num;

  //replacing 0s from the shift right by the bits lost due to it
  unsigned int left = (unsigned int)num << (WORD_SIZE - rot_num);

  return (int)(right | left);
}
