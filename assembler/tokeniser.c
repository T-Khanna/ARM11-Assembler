////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
//
// File: tokeniser.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//-----------------------------  TOKENISER -------------------------------------
// Takes in a line and splits it into tokens. Depending on the value of the
// first token, it will assign a function pointer to the return type. Also will
// also handle calculating the branch offset.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ARMasm.h"


//-- GLOBAL VARIABLES
extern mnemonic_code_mapping table[23];
extern uint32_t (*func_table[32]) (char* []);
extern int is_label(char* token);
extern int label_count;

//-- FUNCTION DECLARATIONS ----------------------------------------------------

void set_pointer(char* code, tokenised* token_line);
tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_index);
tokenised tokeniser(char* line, int line_index);


//-- TOKENISER ----------------------------------------------------------------

void set_pointer(char* code, tokenised* token_line) {
//sets the function pointer in token_line based on the code
  int i = 0;
  char* instr = table[i].mnemonic;
  while (instr) {
    if (strcmp(instr, code) == 0) {
      token_line->func_pointer = func_table[table[i].opcode];
      break;
    }
    i++;
    instr = table[i].mnemonic;
  }
}

void tokenise_init(tokenised* tokenised_str) {
  tokenised_str->func_pointer = NULL;
  for (int i = 0; i < OPERAND_SIZE; i++) {
    tokenised_str->operands[i] = NULL;
  }
}

int in_range(int pos, int pos1, int pos2) {
//checks if pos is between pos1 and pos2
  return (pos >= pos1 && pos <= pos2) || (pos >= pos2 && pos <= pos1);
}

int get_labels_in_between(int label_pos, int line_index) {

  int labels_in_between = 0;

  /*counts how many labels are between the label_pos and line_index
    Note: This includes the original label at label_pos.*/
  for (int i = 0; i < label_count; i++) {
    if (in_range(symb_table[i].position, label_pos, line_index)) {
      labels_in_between++;
    }
  }

  /*checks if there is a forward reference. If it's a backward reference
    we are passing through the label twice (once at the start, second by
    the jump back to the label). If it's not, we are only passing through
    the label once, hence we have to decrement the number of labels in
    between the range.*/
  if (label_pos > line_index) {
    labels_in_between--;
  }

  return labels_in_between;

}

tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_indexum) {

  tokenised tokenised_str;

  //initialise tokenised_str values.
  tokenise_init(&tokenised_str);
  if (is_label(tokens[0])) {
    tokenised_str.operands[0] = tokens[0];
    return tokenised_str;
  }

  /*we are dealing with a mnemonic as the first token, so we need to set
    the function pointer to match the mnemonic.*/
  set_pointer(tokens[0], &tokenised_str);

  //check if mnemonic is a branch instruction.
  if (*tokens[0] == 'b') {

    //check if the label is in symb_table.
    int i = 0;

    while (symb_table[i].label) {

      if (strcmp(symb_table[i].label, tokens[1]) == 0) {
        char line_diff_val[10];
        int line_diff = symb_table[i].position - line_indexum;

        /*because we are subtracting by the line number, we could be
          subtracting lines which only have labels. Lines with labels
          have no effect on the offset, so we need to add the number
          of labels in between the label position and line number.*/
        line_diff += get_labels_in_between(symb_table[i].position,
                                           line_indexum);
        sprintf(line_diff_val, "%d", line_diff);
        tokenised_str.operands[0] = line_diff_val;
        return tokenised_str;

      }

      i++;

    }

  }

  //at this point, we know that a label cannot exist in the tokens
  int num_of_operands = num_of_tokens - 1;

  for (int i = 0; i < num_of_operands; i++) {
    tokenised_str.operands[i] = tokens[i + 1];
  }

  return tokenised_str;

}

tokenised tokeniser(char *line, int line_indexum) {

  //declare deliminator characters
  char delim1[] = ",", delim2[] = " ";

  //method for storing tokens in an array
  char* tokens[TOKEN_LIMIT], *save_ptr;
  char *temp = strtok_r(line, delim2, &save_ptr);
  int num_of_tokens = 0;

  while (temp != NULL && num_of_tokens < sizeof(tokens)) {

    //remove leading whitespace, if it exists
    if (isspace(*temp)) {
      temp++;
    }

    tokens[num_of_tokens] = temp;
    temp = strtok_r(NULL, delim1, &save_ptr);
    num_of_tokens++;

  }

  return get_tokenised(tokens, num_of_tokens, line_indexum);
}
