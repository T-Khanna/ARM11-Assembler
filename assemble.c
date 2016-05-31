<<<<<<< HEAD
///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: assemble.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMasm.h"


//-- FUNCTION DECLARATIONS ----------------------------------------------------

int32_t* get_instr(char* path);
int32_t* translate_instr(int32_t* assem_instr);
void write_bin(char* path, int32_t* bin_instr);

//-- GLOBAL VARIABLES ---------------------------------------------------------

Symbol_table symbol_table;

//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  // Checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }


  int32_t* assem_instr = get_instr(argv[1]);

  // Performing the pass over the file
  int32_t* bin_instr = translate_instr(assem_instr);

  // Creating output binary file
  write_bin(argv[2], bin_instr);
  return EXIT_SUCCESS;

}


int32_t* get_instr(char* path) {

  // Open source assembly file
  FILE *fptr = fopen(path, "r");

  // Check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open input file\n");
    return NULL;
  }

  fclose(fptr);
  return NULL;
}

int32_t* translate_instr(int32_t* assem_instr) {

  // Return an array of 32 bit words to be written into binary file

  return NULL;

}

void write_bin(char *path, int32_t* bin_instr) {

  // Creating output binary file
  FILE *fptr = fopen(path, "w+b");

  fclose(fptr);
  
}

