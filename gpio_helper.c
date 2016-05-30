////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: gpio_helper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////

#include "ARMgen.h"


//-- FUNCTION DECLARATIONS -----------------------------------------------------

void setGPIOAddr();
void clearGPIOAddr();
int convGPIOAddr();


//-- FUNCTIONS -----------------------------------------------------------------

//sets addresses as the values of the GPIO pins
void setGPIOAddr(current_state* cur_state) {
  
	cur_state->memory[MEMORY_CAPACITY - 1] = 0x20200008;
	cur_state->memory[MEMORY_CAPACITY - 2] = 0x20200004;
	cur_state->memory[MEMORY_CAPACITY - 3] = 0x20200000;

}

//clears addresses in memory of the GPIO pins
void clearGPIOAddr(current_state* cur_state) {

	cur_state->memory[MEMORY_CAPACITY - 1] = 0;
	cur_state->memory[MEMORY_CAPACITY - 2] = 0;
	cur_state->memory[MEMORY_CAPACITY - 3] = 0;

}

//converts address of GPIO pins to last memory addresses
int convGPIOAddr() {
  
  return 0;
}


