#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ARMasm.h"

int label_count = 0;
extern mnemonic_code_mapping table[23];
extern int32_t (*func_table[32]) (int32_t[]);
 
//-- FUNCTION DECLARATIONS ----------------------------------------------------

int get_operands(char* operand, int size);
void set_pointer(char* code, tokenised token_line);
tokenised get_tokenised(char* tokens[TOKEN_LIMIT], int num_of_tokens);
tokenised tokeniser(char* line);

//-- TOKENISER ----------------------------------------------------------------

int get_operands(char* operand, int size) {
  for (int i = 0; i < size; i++) {
    if ((operand[i] >= '0' && operand[i] <= '9') || operand[i] == '-') {
      return (int) strtol(operand + i, NULL, 0);
    }
  }
  return 0;
}

void set_pointer(char* code, tokenised token_line) {
  int i = 0;
  char* instr = table[i].mnemonic;
  while (instr) {
    if (strcmp(instr, code) == 0) {
      token_line.func_pointer = func_table[table[i].opcode];
      break;
    }
    i++;
    instr = table[i].mnemonic;
  }
}

// token_line.func_pointer = op_table[i].op_func_pointer;

tokenised get_tokenised(char *tokens[TOKEN_LIMIT], int num_of_tokens) {
  tokenised tokenised_str;
  // Initialise tokenised_str values
  tokenised_str.label = NULL;  
  tokenised_str.func_pointer = NULL;  
  for (int i = 0; i < OPERAND_SIZE; i++) {
    tokenised_str.operands[i] = 0;
  }
  // Check if the first token is a label or not
  if (*(tokens[0] + strcspn(tokens[0], ":")) == ':' && isalpha(*tokens[0])) {
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
    return tokenised_str;
  } 
  set_pointer(tokens[0], tokenised_str);
  if (*tokens[0] == 'b') {
    // Mnemonic is a branch instruction
    // TODO: Need to find a way of getting the address of the label
  } else {
    // At this point, we know that a label cannot exist in the tokens
    int num_of_operands = num_of_tokens - 1;
    for (int i = 0; i < num_of_operands; i++) {
      tokenised_str.operands[i] = get_operands(tokens[i+1], num_of_operands);
    }
  }
  return tokenised_str;
}

tokenised tokeniser(char *line) {
  // Declare deliminator characters
  const char delims[] = " ,";
  
  // Method for storing tokens in an array
  char* tokens[TOKEN_LIMIT], *save_ptr;
  char *temp = strtok_r(line, delims, &save_ptr);
  int num_of_tokens = 0;
  while (temp != NULL && num_of_tokens < sizeof(tokens)) {
    tokens[num_of_tokens] = temp;
    temp = strtok_r(NULL, delims, &save_ptr); 
    num_of_tokens++;
  }
  
  return get_tokenised(tokens, num_of_tokens);
}
