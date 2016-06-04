////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: bitOper.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//---------------------- BITWISE OPERATIONS HEADER------------------------------
// Contains the function definitions for bitOper.c to perform bitwise 
// operations on the instruction.

#ifndef _BIT_OPER
  #define _BIT_OPER



//-- FUNCTION DECLARATIONS -----------------------------------------------------

int getBit(int* num, int index);
void setBit(int* num, int givenBit, int index);
int getBits(int* num, int index, int size);
void setBits(int* target, int index_target, int* source, int index_source, 
             int size);
int rotateRight(int num, unsigned int rot_num);
int decToBin(int n);


#endif

