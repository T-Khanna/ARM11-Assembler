////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
// 
// File: gpio_helper.h
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//-------------------------- GPIO HELPER HEADER --------------------------------
// Contains the function definitions for emulate.c, the execute function in 
// particular to carry out the four instructions specified.

#include "ARMgen.h"



//-- FUNCTION DECLARATIONS -----------------------------------------------------

void setGPIOAddr(current_state* cur_state);
void clearGPIOAddr(current_state* cur_state);
int convGPIOAddr(int* addr);


