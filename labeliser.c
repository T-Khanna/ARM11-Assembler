#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "ARMasm.h"

int label_count = 0;
 
//-- FUNCTION DECLARATIONS ----------------------------------------------------

void labeliser(char* line, int line_num);
void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines);

//-- LABELISER ----------------------------------------------------------------

// Check if the first token is a label or not.
int is_label(char* token) {
  return *(token + strcspn(token, ":")) == ':' && isalpha(*token);
}

void labeliser(char *line, int line_num) {
  char delim[] = " ", *save_ptr, *token = strtok_r(line, delim, &save_ptr);
  // Check for label
  if (is_label(token)) {
    // Update symbol table
    strcpy(symb_table[label_count].label, token);
    symb_table[label_count].position = line_num;
    
    // Remove colon from label
    int colon_pos = strcspn(symb_table[label_count].label, ":");
    *(symb_table[label_count].label + colon_pos) = '\0';
    
    // Increment the label count
    label_count++; 
  }
}

void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines) {
  // Need to use a local variable in order to avoid changing assem_instrs
  char* token = malloc(CHAR_LIMIT * sizeof(char));
  for (int line_num = 1; line_num <= num_of_lines; line_num++) {
    strcpy(token, assem_instrs[line_num - 1]);
    labeliser(token, line_num);
  }
  free(token);
}
