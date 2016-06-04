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
uint32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length);
void write_bin(char* path, uint32_t* bin_instr, int lines_in_file);
tokenised tokeniser(char *line, int line_num);
uint32_t command_processor(tokenised input);

//-- GLOBAL VARIABLES ---------------------------------------------------------

int label_count;
uint32_t *bin_instr;
int num_of_lines;
int line_num;
int extra_data;

//TODO: ADD FUNC POINTER DATABASE
uint32_t (*func_table[32]) (char* operands[]) = {
  &ASMand, &ASMeor, &ASMsub, &ASMrsb, &ASMadd, &ASMldr, &ASMstr, NULL,
  &ASMtst, &ASMteq, &ASMcmp, NULL, &ASMorr, &ASMmov, &ASMmul, &ASMmla,
  &ASMbeq, &ASMbne, &ASMlsl, &ASMandeq, NULL, NULL, NULL, NULL, NULL,
  NULL, &ASMbge, &ASMblt, &ASMble, &ASMb, NULL
}; 

mnemonic_code_mapping table[23] = {
  // Data Processing
  {"add", ADD},
  {"sub", SUB},
  {"rsb", RSB},
  {"and", AND},
  {"eor", EOR},
  {"orr", ORR},
  {"mov", MOV},
  {"tst", TST},
  {"teq", TEQ},
  {"cmp", CMP},

  // Multiply
  {"mul", MUL},
  {"mla", MLA},
 
  // Single Data Transfer
  {"ldr", LDR},
  {"str", STR},
 
  // Branch 
  {"beq", BEQ}, 
  {"bne", BNE}, 
  {"bge", BGE}, 
  {"blt", BLT}, 
  {"bgt", BGT}, 
  {"ble", BLE}, 
  {"b", B},  
  
  // Special
  {"lsl", LSL},
  {"andeq", ANDEQ}

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
  num_of_lines = get_instrs(argv[1], instrs);
  
  //TODO: Testing
  //for (int i = 0; i < num_of_lines; i++) {
  //  puts(instrs[i]);
  //}

  //performing the pass over the file to decode into binary that will be written
  bin_instr = translate_instr(instrs, num_of_lines);
  
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


//return an array of 32 bit words to be written into binary file
uint32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], 
                         int length_in_lines) {
  
  char* current_instruction;
  tokenised token_line;
  static uint32_t bin_instr[MAX_LINES];
  extra_data = 0;

  for (line_num = 0; line_num < length_in_lines; line_num++) {
    current_instruction = assem_instr[line_num];
    token_line = tokeniser(current_instruction, line_num);
    bin_instr[line_num] = command_processor(token_line);
  }

  // subract number of labels lines from total lines to store only the number
  // of valid output lines as num_of_lines variable
  num_of_lines -= label_count;
  
  return bin_instr;

}

uint32_t command_processor(tokenised input) {
   return (*input.func_pointer)(input.operands);
}


//writes the array of 32 bit words (instructions) into the binary file 
//specified
void write_bin(char *path, uint32_t* bin_instr, int lines_in_file) {

  // Creating output binary file
  FILE *fptr = fopen(path, "w+");
 
  //for(int line = 0; line < lines_in_file; line++){
  //  uint32_t num = bin_instr[line];
  //  uint32_t result = 0;
  //  for(int i = 0; i < INSTRUCTION_BYTE_SIZE; i++){
  //    result = result | (getBits((int32_t*)&num, i * BYTE_SIZE, BYTE_SIZE) //gets a byte
  //             << ((INSTRUCTION_BYTE_SIZE -1 - i) * BYTE_SIZE));
  //  }
  //  bin_instr[line] = result;
  //}

  fwrite(bin_instr, INSTRUCTION_BYTE_SIZE, lines_in_file, fptr);

  //closing file
  fclose(fptr);

  
}



