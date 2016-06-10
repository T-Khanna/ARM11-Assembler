///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: labeliser.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "ARMasm.h"

int label_count = 0;
 
//-- FUNCTION DECLARATIONS ----------------------------------------------------

void labeliser(char* line, int line_n);
void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines);

//-- LABELISER ----------------------------------------------------------------

//check if the first token is a label or not.
int is_label(char* token) {
  return token != NULL && *(token + strcspn(token, ":")) == ':' 
            && isalpha(*token);
}

void labeliser(char *line, int line_n) {
  char delim[] = " ", *save_ptr;
  char* token = strtok_r(line, delim, &save_ptr);
  //check for label
  if (is_label(token)) {
    //update symbol table
    strcpy(symb_table[label_count].label, token);
    symb_table[label_count].position = line_n;
    
    //remove colon from label
    int colon_pos = strcspn(symb_table[label_count].label, ":");
    *(symb_table[label_count].label + colon_pos) = '\0';
    
    //increment the label count
    label_count++; 
  }
}

void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines) {
  //need to use a local variable in order to avoid changing assem_instrs
  char* token = malloc(CHAR_LIMIT * sizeof(char));
  for (int line_n = 1; line_n <= num_of_lines; line_n++) {
    strcpy(token, assem_instrs[line_n - 1]);
    labeliser(token, line_n);
  }
  free(token);
}
