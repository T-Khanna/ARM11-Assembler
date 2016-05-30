///////////////////////////////////////////////////////////////////////////////
//ARM Group Project - Year 1 (Group 40)
//_____________________________________________________________________________
//
//File: assemble.c
//Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMasm.h"


//-- FUNCTION DECLARATIONS ----------------------------------------------------

void symbol_table_init(FILE *fptr);
int32_t* create_bin_inst(FILE *fptr);
void write_bin(char *file_name, int32_t *instr, int num_of_instrs);

//-- GLOBAL VARIABLES ---------------------------------------------------------

Symbol_table symbol_table;

//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  // Checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }

  // Open source assembly file
  FILE *fptr = fopen(argv[0], "r");

  // Check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  // Performing the first pass over the file
  symbol_table_init(fptr);

  // Performing the second pass over the file
  int32_t *inst = create_bin_inst(fptr);
  
  // Creating output binary file
  int num_of_instrs;
  write_bin(argv[1], inst, num_of_instrs);
  
  return EXIT_SUCCESS;

}

void symbol_table_init(FILE *fptr) {

  // Use tokenizer (strtok_r and strtol)

  // Make sure to store the results in symbol_table

}

int32_t* create_bin_inst(FILE *fptr) {

  // Return an array of 32 bit words to be written into binary file

  return NULL;

}

void write_bin(char *file_name, int32_t *instr, int num_of_instrs) {

  // Creating output binary file
  FILE *fptr = fopen(file_name, "w+b");

  // Write to output file
  for (int i = 0; i < num_of_instrs; i++) {
    fwrite(&(instr[i]), sizeof(int32_t), 1, fptr);  
  }

  fclose(fptr);

}
