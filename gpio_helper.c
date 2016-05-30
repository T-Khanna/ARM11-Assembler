////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: gpio_helper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "ARMgen.h"
#include "gpio_helper.h"



//-- CONSTANTS -----------------------------------------------------------------

#define GPIO_ADDR_0_9			      0x20200000
#define GPIO_ADDR_10_19		    	0x20200004
#define GPIO_ADDR_20_29			    0x20200008
#define GPIO_ADDR_SET		  	    0x2020001C
#define GPIO_ADDR_CLEAR	    		0x20200028


//-- FUNCTIONS -----------------------------------------------------------------

//sets addresses as the values of the GPIO pins
void setGPIOAddr(current_state* cur_state) {
  
	cur_state->memory[MEMORY_CAPACITY - 3] = GPIO_ADDR_0_9;
	cur_state->memory[MEMORY_CAPACITY - 2] = GPIO_ADDR_10_19;
	cur_state->memory[MEMORY_CAPACITY - 1] = GPIO_ADDR_20_29;

}

//clears addresses in memory of the GPIO pins
void clearGPIOAddr(current_state* cur_state) {

	cur_state->memory[MEMORY_CAPACITY - 1] = 0;
	cur_state->memory[MEMORY_CAPACITY - 2] = 0;
	cur_state->memory[MEMORY_CAPACITY - 3] = 0;

}

//converts address of GPIO pins to last memory addresses
int convGPIOAddr(int* addr) {

  switch(*addr) {

    //SET PIN ADDRESS
    case GPIO_ADDR_SET:
      printf("PIN ON\n");
      return 1;

		//CLEAR PIN ADDRESS
		case GPIO_ADDR_CLEAR:
			printf("PIN OFF\n");
			return 1;

    //GPIO ports 0-9 accesssed
    case GPIO_ADDR_0_9:
      printf("One GPIO pin from 0 to 9 has been accessed\n");
      break;

		//GPIO ports 10-19 accesssed
		case GPIO_ADDR_10_19:
			printf("One GPIO pin from 10 to 19 has been accessed\n");
			break;
    
		//GPIO ports 20-29 accesssed
		case GPIO_ADDR_20_29:
    	printf("One GPIO pin from 20 to 29 has been accessed\n");
			break;

		default:
			return 0;

	}

	*addr = ((MEMORY_CAPACITY - ((*addr - GPIO_ADDR_0_9) / 4)) - 1) * 4;
	return 0;
}


