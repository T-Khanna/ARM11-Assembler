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
#include <ctype.h>
#include "ARMasm.h"
#include "bitOper.h"

//-- FUNCTION DECLARATIONS ----------------------------------------------------

int get_instrs(char* path, char instrs[MAX_LINES][CHAR_LIMIT]);
int32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length);
void write_bin(char* path, int32_t* bin_instr, int lines_in_file);
tokenised tokeniser(char* line);

//-- GLOBAL VARIABLES ---------------------------------------------------------

extern int label_count;

//TODO: ADD FUNC POINTER DATABASE
int32_t (*func_table[32]) (int32_t[]) = {
  &ASMand, &ASMeor, &ASMsub, &ASMrsb, &ASMadd, &ASMldr, &ASMstr, NULL,
  &ASMtst, &ASMteq, &ASMcmp, NULL, &ASMorr, &ASMmov, &ASMmul, &ASMmla,
  &ASMbeq, &ASMbne, &ASMlsl, &ASMandeq, NULL, NULL, NULL, NULL, NULL,
  NULL, &ASMbge, &ASMblt, &ASMble, &ASMb, NULL
}; 

mnemonic_code_mapping table[23] = {
  // Data Processing
  {"add", 4},
  {"sub", 2},
  {"rsb", 3},
  {"and", 0},
  {"eor", 1},
  {"orr", 12},
  {"mov", 13},
  {"tst", 8},
  {"teq", 9},
  {"cmp", 10},

  // Multiply
  {"mul", 14},
  {"mla", 15},
 
  // Single Data Transfer
  {"ldr", 5},
  {"str", 6},
 
  // Branch 
  {"beq", 16}, 
  {"bne", 17}, 
  {"bge", 26}, 
  {"blt", 27}, 
  {"bgt", 28}, 
  {"ble", 29}, 
  {"b", 30},  
  
  // Special
  {"lsl", 18},
  {"andeq", 19}

};


//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  //checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }

  //getting the instruction from source file into an array of 32-bit
  //instructions that will be translated.
  char instrs[MAX_LINES][CHAR_LIMIT];
  int num_of_lines = get_instrs(argv[1], instrs);
  
  for (int i = 0; i < num_of_lines; i++) {
    puts(instrs[i]);
  }

  //performing the pass over the file to decode into binary that will be written
  int32_t* bin_instr = translate_instr(instrs, num_of_lines);
  
  //creating output binary file
  write_bin(argv[2], bin_instr, num_of_lines);
  
  return EXIT_SUCCESS;

}


//-- FUNCTION DEFINTIONS -------------------------------------------------------

//gets instructions from source file into an array of 32-bit instructions
// Also returns the number of lines to preven segmentation fault
int get_instrs(char* path, char instrs[MAX_LINES][CHAR_LIMIT]) {

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
  while (fgets(instrs[lines_in_file], CHAR_LIMIT, fptr)) {

    //having loaded current line into the array of instructions specified, we
    //need to replace the '\n' at the end of each line to '\0'

    // find the position of the '\n' new line character that we want to replace
    int newline_pos = strcspn(instrs[lines_in_file], "\n");

    // Set the value to '\0' to terminate the string
    instrs[lines_in_file][newline_pos] = '\0';

    // proceed to the next line in the file
    lines_in_file++;

  }
 
  fclose(fptr);
  return lines_in_file;

}

int32_t command_processor(tokenised input) {
 return (*input.func_pointer)(input.operands);
}

//return an array of 32 bit words to be written into binary file
int32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length) {
  
  char* current_instruction;
  tokenised token_line;
  static int32_t bin_instr[MAX_LINES];

  for (int i = 0; i < length; i++) {
    current_instruction = assem_instr[i];
    token_line = tokeniser(current_instruction);
    bin_instr[i] = command_processor(token_line);
  
  }

  
  return bin_instr;

}

//writes the array of 32 bit words (instructions) into the binary file 
//specified
void write_bin(char *path, int32_t* bin_instr, int lines_in_file) {

  // Creating output binary file
  FILE *fptr = fopen(path, "w+");
 
  // subract number of labels lines from total lines
  lines_in_file -= label_count;

  for(int line = 0; line < lines_in_file; line++){
    int32_t num = bin_instr[line];
    int32_t result = 0;
    for(int i = 0; i < INSTRUCTION_BYTE_SIZE; i++){
      result = result | (getBits(&num, i * BYTE_SIZE, BYTE_SIZE) //gets a byte
               << ((INSTRUCTION_BYTE_SIZE -1 - i) * BYTE_SIZE));
    }
    bin_instr[line] = result;
  }

  fwrite(bin_instr, INSTRUCTION_BYTE_SIZE, lines_in_file, fptr);

  //closing file
  fclose(fptr);

  
}



