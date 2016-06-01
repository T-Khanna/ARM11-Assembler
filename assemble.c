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
tokenised tokeniser(char* line);
int32_t* translate_instr(int32_t* assem_instr);
void write_bin(char* path, int32_t* bin_instr);


//-- GLOBAL VARIABLES ---------------------------------------------------------

struct symbol_table symb_table;


//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  //checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }

  //getting the instruction from source file into an array of 32-bit
  //instructions that will be translated.
  int32_t* assem_instr = get_instr(argv[1]);

  //performing the pass over the file to decode into binary that will be written
  int32_t* bin_instr = translate_instr(assem_instr);

  //creating output binary file
  write_bin(argv[2], bin_instr);
  
  return EXIT_SUCCESS;

}


//-- FUNCTION DEFINTIONS -------------------------------------------------------

//gets instructions from source file into an array of 32-bit instructions
int32_t* get_instr(char* path) {

  // Open source assembly file
  FILE *fptr = fopen(path, "r");

  // Check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open input file\n");
    return NULL;
  }

  //TODO: CODE that turns each instruction in the source file into an array
  //      of 32-bit instructions.

  char line[LIMIT_PER_LINE];
  
  // Better than !feof(fptr) as it actually stops at EOF 
  while (fgets(line, LIMIT_PER_LINE, fptr)) {
    // This removes the trailing '\n' from fgets
    line[strcspn(line, "\n")] = '\0';
    tokeniser(line);
  }
  

  fclose(fptr);
  return NULL;

}

int is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int check_label(char *token) {
  return *(token + strcspn(token, ":")) == ':' && is_letter(*token);
}

tokenised tokeniser(char *line) {
  tokenised tokenised;
  const char delims[] = " ,";
  char *tokens[TOKEN_LIMIT], *save_ptr;
  char* temp = strtok_r(line, delims, &save_ptr);
  int i = 0;
  while (temp != NULL && i < sizeof(tokens)) {
    tokens[i] = temp;
    temp = strtok_r(NULL, delims, &save_ptr); 
    i++;
  }
  if (check_label(tokens[0])) {
    char* label_to_add = tokens[0];
    *(label_to_add + strcspn(label_to_add, ":")) = '\0';
    tokenised.label = label_to_add;
    *(symb_table.label) = label_to_add;
    *(symb_table.memory_address) = (unsigned long) label_to_add;
    printf("%lx\n", *(symb_table.memory_address));
  } else {
    
  }
  return tokenised;
}


//return an array of 32 bit words to be written into binary file
int32_t* translate_instr(int32_t* assem_instr) {

  //TODO: CODE that translates each 32 bit word into binary

  return NULL;

}

//writes the array of 32 bit words (instructions) into the binary file 
//specified
void write_bin(char *path, int32_t* bin_instr) {

  // Creating output binary file
  FILE *fptr = fopen(path, "w+b");

  //TODO: CODE that writes given binary to the file specified

  fclose(fptr);
  
}


