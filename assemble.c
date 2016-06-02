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

int get_instrs(char* path, char instrs[MAX_LINES][CHAR_LIMIT]);
tokenised tokenizer(char* line);
int32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length);
void write_bin(char* path, int32_t* bin_instr, int lines_in_file);


//-- GLOBAL VARIABLES ---------------------------------------------------------

struct symbol_table symb_table[MAX_LABELS];
int label_count = 0;

struct mnemonic_code_mapping op_table[] = {
    
  // Data Processing
  {"add", 4, &ARMadd},
  {"sub", 2, &ARMsub},
  {"rsb", 3, &ARMrsb},
  {"and", 0, &ARMand},
  {"eor", 1, &ARMeor},
  {"orr", 12, &ARMorr},
  {"mov", 13, &ARMmov},
  {"tst", 8, &ARMtst},
  {"teq", 9, &ARMteq},
  {"cmp", 10, &ARMcmp},

  // Multiply
  {"mul", 14, &ARMmul},
  {"mla", 15, &ARMmla},
 
  // Single Data Transfer
  {"ldr", 5, &ARMldr},
  {"str", 6, &ARMstr},
 
  // Branch 
  {"beq", 16, &ARMbeq}, 
  {"bne", 17, &ARMbne}, 
  {"bge", 26, &ARMbge}, 
  {"blt", 27, &ARMblt}, 
  {"bgt", 28, &ARMbgt}, 
  {"ble", 29, &ARMble}, 
  {"b", 30, &ARMb},  
  
  // Special
  {"lsl", 31, &ARMlsl},
  {"andeq", 32, &ARMandeq}
  
    
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

int is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int get_code(char* code) {
  int i = 0;
  char* instr = op_table[i].mnemonic;
  while (instr) {
    if (strcmp(instr, code) == 0) {
      return op_table[i].code;
    }
    i++;
    instr = op_table[i].mnemonic;
  }
  return 0;
}

tokenised check_label(char *tokens[TOKEN_LIMIT]) {
  
  tokenised tokenised_str;

  // Initialise tokenised_str values
  tokenised_str.label = 0;  
  // Can't use 0 here as that is the opcode for 'and' 
  tokenised_str.code = 18;  
  for (int i = 0; i < OPERAND_SIZE; i++) {
    tokenised_str.operands[i] = 0;
  } 
  
  // Check if the first token is a label or not
  if (*(tokens[0] + strcspn(tokens[0], ":")) == ':' && is_letter(*tokens[0])) {
    // Line starts with a label  
    char* new_label = tokens[0];
    
    // Remove ':' from label
    *(new_label + strcspn(new_label, ":")) = '\0';
    
    // Update tokenised label variable
    tokenised_str.label = new_label;
    
    // Update label -> address symbol table
    symb_table[label_count].label = new_label;
    symb_table[label_count].memory_address = &new_label;
    
    // Increment number of labels to move the pointer for the next
    // label -> address pair
    label_count++;
  } else {
    tokenised_str.code = get_code(tokens[0]);
    for (int i = 0; i < OPERAND_SIZE; i++) {
      tokenised_str.operands[i] = 0; // Need to use strtol here
    }
  }
  
  return tokenised_str;

}

tokenised tokenizer(char *line) {
  // Declare deliminator characters
  const char delims[] = " ,";
  
  // Method for storing tokens in an array
  char* tokens[TOKEN_LIMIT], *save_ptr;
  char *temp = strtok_r(line, delims, &save_ptr);
  int i = 0;
  while (temp != NULL && i < sizeof(tokens)) {
    tokens[i] = temp;
    temp = strtok_r(NULL, delims, &save_ptr); 
    i++;
  }
  
  return check_label(tokens);
}

int32_t command_Processor(tokenised input) {
  return (*input.func_pointer)(input.operands);
}


//return an array of 32 bit words to be written into binary file
int32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length) {
  
  char* current_instruction;
  tokenised token_line;
  int32_t bin_line;
  static int32_t bin_instr[MAX_LINES];

  for (int i = 0; i < length; i++) {
    current_instruction = assem_instr[i];
    token_line = tokenizer(current_instruction);
    bin_line = command_Processor(token_line);
    bin_instr[i] = bin_line;    
  }

  return bin_instr;

}

//writes the array of 32 bit words (instructions) into the binary file 
//specified
void write_bin(char *path, int32_t* bin_instr, int lines_in_file) {

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



