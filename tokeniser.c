///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: tokeniser.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ARMasm.h"

extern mnemonic_code_mapping table[23];
extern uint32_t (*func_table[32]) (char* []);
extern int is_label(char* token);
extern int label_count;
 
//-- FUNCTION DECLARATIONS ----------------------------------------------------

//int get_operands(char* operand, int size);
void set_pointer(char* code, tokenised* token_line);
tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_num);
tokenised tokeniser(char* line, int line_num);

//-- TOKENISER ----------------------------------------------------------------

// Sets the function pointer in token_line based on the code
void set_pointer(char* code, tokenised* token_line) {
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

// Checks if pos is between pos1 and pos2
int in_range(int pos, int pos1, int pos2) {
  return (pos >= pos1 && pos <= pos2) || (pos >= pos2 && pos <= pos1);
}

int get_labels_in_between(int label_pos, int line_num) {
  int labels_in_between = 0;
  // Counts how many labels are between the label_pos and line_num
  // Note: This includes the original label at label_pos.
  for (int i = 0; i < label_count; i++) {
    if (in_range(symb_table[i].position, label_pos, line_num)) {
      labels_in_between++;
    }
  }
  // Checks if there is a forward reference. If it's a backward reference 
  // we are passing through the label twice (once at the start, second by
  // the jump back to the label). If it's not, we are only passing through
  // the label once, hence we have to decrement the number of labels in
  // between the range.
  if (label_pos > line_num) {
    labels_in_between--;
  }
  return labels_in_between;
}

tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_num) {
  tokenised tokenised_str;

  // Initialise tokenised_str values.
  tokenise_init(&tokenised_str);
  if (is_label(tokens[0])) {
    tokenised_str.operands[0] = tokens[0];
    return tokenised_str;
  }
  // We are dealing with a mnemonic as the first token, so we need to set
  // the function pointer to match the mnemonic. 
  set_pointer(tokens[0], &tokenised_str);
  
  if (*tokens[0] == 'b') {
    // Mnemonic is a branch instruction.
    // Check if the label is in symb_table. 
    int i = 0;
    while (symb_table[i].label) {
      if (strcmp(symb_table[i].label, tokens[1]) == 0) {
        char line_diff_val[10];
        int line_diff = symb_table[i].position - line_num;
        // Because we are subtracting by the line number, we could be 
        // subtracting lines which only have labels. Lines with labels 
        // have no effect on the offset, so we need to add the number
        // of labels in between the label position and line number.
        line_diff += get_labels_in_between(symb_table[i].position, line_num);
        printf(line_diff_val, "%d", line_diff);
        tokenised_str.operands[0] = line_diff_val;
        return tokenised_str;
      }
      i++;
    }
  }
  // At this point, we know that a label cannot exist in the tokens
  int num_of_operands = num_of_tokens - 1;
  for (int i = 0; i < num_of_operands; i++) {
    tokenised_str.operands[i] = tokens[i + 1];
  }
 
  return tokenised_str;
}

tokenised tokeniser(char *line, int line_num) {
  // Declare deliminator characters
  char delim1[] = ",", delim2[] = " ";
  
  // Method for storing tokens in an array
  char* tokens[TOKEN_LIMIT], *save_ptr;
  char *temp = strtok_r(line, delim2, &save_ptr);
  int num_of_tokens = 0;

  while (temp != NULL && num_of_tokens < sizeof(tokens)) {
    // Remove leading whitespace, if it exists
    if (isspace(*temp)) {
      temp++;
    }
    tokens[num_of_tokens] = temp;
    temp = strtok_r(NULL, delim1, &save_ptr); 
    num_of_tokens++;
  }

  return get_tokenised(tokens, num_of_tokens, line_num);
}
