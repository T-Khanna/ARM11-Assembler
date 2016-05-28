///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: bitOper.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////
#ifndef _BIT_OPER
  #define _BIT_OPER

//-- FUNCTION DECLARATIONS ---------------------------------------------------

int getBit(int* num, int index);
void setBit(int* num, int givenBit, int index);
int getBits(int* num, int index, int size);
void setBits(int* target, int index_target, int* source, int index_source, 
             int size);
int rotate_right(int num, unsigned int rot_num);
#endif
