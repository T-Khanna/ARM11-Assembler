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

int get_instrs(char* path, char instrs[MAX_LINES][LIMIT_PER_LINE]);
tokenised tokeniser(char* line);
int32_t* translate_instr(char assem_instr[MAX_LINES][LIMIT_PER_LINE]);
void write_bin(char* path, int32_t* bin_instr);


//-- GLOBAL VARIABLES ---------------------------------------------------------

struct symbol_table symb_table;
int lines_in_file;


//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  //checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }

  //getting the instruction from source file into an array of 32-bit
  //instructions that will be translated.
  char instrs[MAX_LINES][LIMIT_PER_LINE];
  int num_of_lines = get_instrs(argv[1], instrs);
  
  for (int i = 0; i < num_of_lines; i++) {
    puts(instrs[i]);
  }

  //performing the pass over the file to decode into binary that will be written
  int32_t* bin_instr = translate_instr(instrs);

  //creating output binary file
  write_bin(argv[2], bin_instr);
  
  return EXIT_SUCCESS;

}


//-- FUNCTION DEFINTIONS -------------------------------------------------------

//gets instructions from source file into an array of 32-bit instructions
// Also returns the number of lines to preven segmentation fault
int get_instrs(char* path, char instrs[MAX_LINES][LIMIT_PER_LINE]) {

  // Open source assembly file
  FILE *fptr = fopen(path, "r");

  // Check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open input file\n");
    return 0;
  }

  int lines_in_file = 0;

  //Loads each line into the array of instructions specified
  // enters loop if current line exists and reading it is succesful. 
  // breaks loop when we reach the end of the file.
  while (fgets(instrs[lines_in_file], LIMIT_PER_LINE, fptr)) {
<<<<<<< Updated upstream
    // This replaces the '\n' character with '\0' 
    // so that it terminates the string
    instrs[lines_in_file][strcspn(instrs[lines_in_file], "\n")] = '\0';
=======

    //having loaded current line into the array of instructions specified, we
    //need to replace the '\n' at the end of each line to '\0'

    // find the position of the '\n' new line character that we want to replace
    int newline_pos = strcspn(instrs[lines_in_file], "\n");

    // replace the value at that position to '\0' to terminate the string
    instrs[lines_in_file][newline_pos] = '\0';

    // proceed to the next line in the file
>>>>>>> Stashed changes
    lines_in_file++;

  }
 
  fclose(fptr);
  return lines_in_file;

}

int is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int check_label(char *token) {
  return *(token + strcspn(token, ":")) == ':' && is_letter(*token);
}

tokenised tokeniser(char *line) {
  tokenised tokenised_str;

  tokenised_str.label = NULL;

  const char delims[] = " ,";
  char* tokens[TOKEN_LIMIT], *save_ptr;
  char *temp = strtok_r(line, delims, &save_ptr);
  int i = 0;
  while (temp != NULL && i < sizeof(tokens)) {
    tokens[i] = temp;
    temp = strtok_r(NULL, delims, &save_ptr); 
    i++;
  }
  if (check_label(tokens[0])) {
    char* label_to_add = tokens[0];
    *(label_to_add + strcspn(label_to_add, ":")) = '\0';
    tokenised_str.label = label_to_add;
    *(symb_table.label) = label_to_add;
    *(symb_table.memory_address) = (unsigned long) label_to_add;
    printf("%lx\n", *(symb_table.memory_address));
  } else {
    
  }
  return tokenised_str;
}


//return an array of 32 bit words to be written into binary file
int32_t* translate_instr(char assem_instr[MAX_LINES][LIMIT_PER_LINE]) {

  //TODO: CODE that translates each 32 bit word into binary
  //while(not end of file) {
  //  currentInstruction = (correct 32bit int in assem_instr array/pointer);
  //  tokenLine = tokenise(currentInstruction);
  //  binLine = commandProcessing(tokenLine);
  //  append(bin instrs, binline);
  //  go to next line i++
  //}

  //i = 0;
  //while(i < lines_in_file){

  //  int32_t* currentInstruction;
  //  
  //  
  //}

  return NULL;

}

//writes the array of 32 bit words (instructions) into the binary file 
//specified
void write_bin(char *path, int32_t* bin_instr) {

  // Creating output binary file
  FILE *fptr = fopen(path, "w+b");

  for(int i = 0; i < lines_in_file; i++){
    fwrite((bin_instr + i * INSTRUCTION_BIT_SIZE), 1, 
           INSTRUCTION_BIT_SIZE, fptr);
    fputs("\n", fptr);
  }

  //closing file
  fclose(fptr);
  
}



