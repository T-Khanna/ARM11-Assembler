///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: assemble.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////


//-------------------------------- ASSEMBLE ------------------------------------
// Holds the program that converts assembly code into ARM binary code for the
// raspberry pi to understand

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ARMasm.h"
#include "../bitOpers/bitOper.h"



//-- FUNCTION DECLARATIONS ----------------------------------------------------

void symbol_table_init();
int get_instrs(char* path, char instrs[MAX_LINES][CHAR_LIMIT]);
void store_labels(char assem_instr[MAX_LINES][CHAR_LIMIT], int num_of_lines);
uint32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT], int length);
void write_bin(char* path, uint32_t* bin_instr, int lines_in_file);
tokenised tokeniser(char *line, int line_n);
uint32_t command_processor(tokenised input);
void free_symbol_table();


//-- GLOBAL VARIABLES ---------------------------------------------------------

extern int label_count;
extern int is_label(char* token);
uint32_t *bin_instr;
uint32_t bin_instr_curr[MAX_LINES];
int num_of_lines;
int line_num;
int valid_line_num;
int extra_data;

uint32_t (*func_table[32]) (char* operands[]) = {
  &ASMand, &ASMeor, &ASMsub, &ASMrsb, &ASMadd, &ASMldr, &ASMstr, NULL,
  &ASMtst, &ASMteq, &ASMcmp, NULL, &ASMorr, &ASMmov, &ASMmul, &ASMmla,
  &ASMbeq, &ASMbne, &ASMlsl, &ASMandeq, NULL, NULL, NULL, NULL, NULL,
  NULL, &ASMbge, &ASMblt, &ASMbgt, &ASMble, &ASMb, NULL
};

mnemonic_code_mapping table[23] = {
  //data Processing
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

  //multiply
  {"mul", MUL},
  {"mla", MLA},

  //single Data Transfer
  {"ldr", LDR},
  {"str", STR},

  //branch
  {"beq", BEQ},
  {"bne", BNE},
  {"bge", BGE},
  {"blt", BLT},
  {"bgt", BGT},
  {"ble", BLE},
  {"b", B},

  //special
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

  //initialise symbol table
  symbol_table_init();

  //getting the instruction from source file into an array of 32-bit
  //instructions that will be translated.
  char instrs[MAX_LINES][CHAR_LIMIT];
  num_of_lines = get_instrs(argv[1], instrs);

  //storing the label locations in the symbol table
  store_labels(instrs, num_of_lines);

  //performing the pass over the file to decode into binary that will be written
  bin_instr = translate_instr(instrs, num_of_lines);

  //creating output binary file
  write_bin(argv[2], bin_instr, num_of_lines);

  //free memory used in symbol_table
  free_symbol_table();

  return EXIT_SUCCESS;

}


//-- FUNCTION DEFINTIONS -------------------------------------------------------

void symbol_table_init() {
//initialised symbol table
  for (int i = 0; i < MAX_LABELS; i++) {
    symb_table[i].label = malloc(CHAR_LIMIT * sizeof(char));
  }
}


int get_instrs(char* path, char instrs[MAX_LINES][CHAR_LIMIT]) {
//gets instructions from source file into an array of 32-bit instructions
//also returns the number of lines to provide a limit for the array

  //open source assembly file
  FILE *fptr = fopen(path, "r");

  //check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open input file\n");
    return 0;
  }

  int lines_in_file = 0;

  //loads each line into the array of instructions specified
  //enters loop if current line exists and reading it is succesful.
  //breaks loop when we reach the end of the file.
  while (fgets(instrs[lines_in_file], CHAR_LIMIT, fptr)) {

    //check if line is empty or is a comment
    if (*instrs[lines_in_file] == '\n' || *instrs[lines_in_file] == ';') {
      continue;
    }

    //having loaded current line into the array of instructions specified, we
    //need to replace the '\n' at the end of each line to '\0'

    //Find the position of the '\n' new line character that we want to replace
    int newline_pos = strcspn(instrs[lines_in_file], "\n");

    //set the value to '\0' to terminate the string
    instrs[lines_in_file][newline_pos] = '\0';

    //proceed to the next line in the file
    lines_in_file++;

  }

  fclose(fptr);
  return lines_in_file;

}


uint32_t* translate_instr(char assem_instr[MAX_LINES][CHAR_LIMIT],
                         int length_in_lines) {
//return an array of 32 bit words to be written into binary file

  char* current_instruction;
  tokenised token_line;

  //used to ensure that the array does not have an empty
  int bin_instr_num = 0;

  //the line number that the extra data from Single Data Transfer
  //will be written to in the output.*/
  valid_line_num = 1;

  for (line_num = 1; line_num <= length_in_lines; line_num++) {

    //read a line from the array
    current_instruction = assem_instr[line_num - 1];

    //tokenise the line
    token_line = tokeniser(current_instruction, line_num);

    //we check if the line is only a label.
    if (is_label(token_line.operands[0])) {
      continue;
    }

    //store the binary instruction in the array.
    bin_instr_curr[bin_instr_num] = command_processor(token_line);

    //move to the next instruction
    bin_instr_num++;
    valid_line_num++;

  }

  //subract number of labels amd empty lines from total lines to store only the
  //number of valid output lines as num_of_lines variable
  num_of_lines -= label_count;

  return bin_instr_curr;

}

uint32_t command_processor(tokenised input) {
//processes each instruction

  return (*input.func_pointer)(input.operands);

}


void write_bin(char *path, uint32_t* bin_instr, int lines_in_file) {
//writes an array of 32 bit words (instructions) into the binary file specified

  //creating output binary file
  FILE *fptr = fopen(path, "wb");

  fwrite(bin_instr, INSTRUCTION_BYTE_SIZE, lines_in_file, fptr);

  //closing file
  fclose(fptr);

}

void free_symbol_table() {
//frees memory initially allocated for labels

  for (int i = 0; i < MAX_LABELS; i++) {
    free(symb_table[i].label);
  }

}
