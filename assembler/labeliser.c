///////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// ____________________________________________________________________________
//
// File: labeliser.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////


//------------------------------ LABELISER -------------------------------------
// Assigns each label to its relative position in the source file.

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "ARMasm.h"

//-- GLOBAL VARIABLES ----------------------------------------------------------

int label_count = 0;


//-- FUNCTION DECLARATIONS -----------------------------------------------------

void labeliser(char* line, int line_index);
void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines);


//-- LABELISER -----------------------------------------------------------------

int is_label(char* token) {
//check if the first token is a label or not

  return token != NULL && *(token + strcspn(token, ":")) == ':'
                       && isalpha(*token);

}

void labeliser(char *line, int line_index) {
//checks for a label within the line and updates symbol table accordingly

  char delim[] = " ", *save_ptr;
  char* token = strtok_r(line, delim, &save_ptr);

  //check for label
  if (is_label(token)) {
    //update symbol table
    strcpy(symb_table[label_count].label, token);
    symb_table[label_count].position = line_index;

    //remove colon from label
    int colon_pos = strcspn(symb_table[label_count].label, ":");
    *(symb_table[label_count].label + colon_pos) = '\0';

    //increment the label count
    label_count++;

  }

}

void store_labels(char assem_instrs[MAX_LINES][CHAR_LIMIT], int num_of_lines) {
//need to use a local variable in order to avoid changing assem_instrs

  char* line = malloc(CHAR_LIMIT * sizeof(char));

  //copies the first line into a local variable and then labeliser checks if
  //that line has a label or not
  for (int line_index = 1; line_index <= num_of_lines; line_index++) {

    strcpy(line, assem_instrs[line_index - 1]);
    labeliser(line, line_index);

  }

  free(line);

}
