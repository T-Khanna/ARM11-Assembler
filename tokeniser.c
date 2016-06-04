#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ARMasm.h"
#include "double_Linked_List.c"

int label_count = 0;
extern mnemonic_code_mapping table[23];
extern uint32_t (*func_table[32]) (char* []);
list label_list;
 
//-- FUNCTION DECLARATIONS ----------------------------------------------------

//int get_operands(char* operand, int size);
void set_pointer(char* code, tokenised* token_line);
tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_num);
tokenised tokeniser(char* line, int line_num);

//-- TOKENISER ----------------------------------------------------------------
/*
int get_operands(char* operand, int size) {
  for (int i = 0; i < size; i++) {
    if ((operand[i] >= '0' && operand[i] <= '9') || operand[i] == '-') {
      return (int) strtol(operand + i, NULL, 0);
    }
  }
  return 0;
}
*/
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

// token_line.func_pointer = op_table[i].op_func_pointer;

void tokenise_init(tokenised* tokenised_str) {
  tokenised_str->label = NULL;  
  tokenised_str->func_pointer = NULL;  
  for (int i = 0; i < OPERAND_SIZE; i++) {
    tokenised_str->operands[i] = "";
  }
}

// Check if the first token is a label or not.
int is_label(char* token) {
  return *(token + strcspn(token, ":")) == ':' && isalpha(*token);
}

tokenised get_tokenised(char* tokens[TOKEN_LIMIT],
                              int num_of_tokens, int line_num) {
  tokenised tokenised_str;

  // Initialise tokenised_str values.
  tokenise_init(&tokenised_str);

  // Initialise label_list.
  list_init(&label_list);

  if (is_label(tokens[0])) {
    // Line starts with a label.  
    char* new_label = tokens[0];
    
    // Remove ':' from label.
    *(new_label + strcspn(new_label, ":")) = '\0';
    
    // Update tokenised label variable.
    tokenised_str.label = new_label;
    tokenised_str.label_pos = line_num;
    
    // Update label->address in the symbol table for resolving any futures
    // backward references.
    symb_table[label_count].label = new_label;
    symb_table[label_count].position = line_num;
 
    // Update label_list and check for forward references to
    // previously undeclared labels.
    list_iter iter = list_end(&label_list);
    // Iterating from the end to the front of the list.
    while (iter != label_list.header) {
      if (strcmp(iter->value.label, new_label) == 0) {
        // We need to update the label position for this value.
        iter->value.label_pos = line_num;
        list_remove(&label_list, iter);
      } else {
        list_iter_prev(iter);
      }   
    }
 
    // Increment number of labels to move the pointer for the next
    // label -> address pair
    label_count++;
  } else {
    // We are dealing with a mnemonic as the first token, so we need to set
    // the function pointer to match the mnemonic. 
    set_pointer(tokens[0], &tokenised_str);
    
    if (*tokens[0] == 'b') {
      // Mnemonic is a branch instruction.

      // Check if the label is in symb_table. 
      int i = 0;
      while (symb_table[i].label) {
        if (strcmp(symb_table[i].label, tokens[1]) == 0) {
          char offset_val[10];
          int offset = (line_num - symb_table[i].position) 
                          * BYTE_SIZE - PIPELINE_OFFSET;
          sprintf(offset_val, "%d", offset);
          tokenised_str.operands[0] = offset_val;
        }
      }
      
      if (tokenised_str.operands[0] == NULL) {
        // Label is a forward reference to an unknown address
      }

    } else {
      // At this point, we know that a label cannot exist in the tokens
      int num_of_operands = num_of_tokens - 1;
      for (int i = 0; i < num_of_operands; i++) {
        tokenised_str.operands[i] = tokens[i + 1];
      }
    }
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
