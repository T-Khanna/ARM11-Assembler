///////////////////////////////////////////////////////////////////////////////
//ARM Group Project - Year 1 (Group 40)
//_____________________________________________________________________________
//
//File: assemble.c
//Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////



#include <stdlib.h>
#define LINE_LIMIT 511


//-- FUNCTION DECLARATIONS ----------------------------------------------------

void read_ASM(char *source_code);
void write_bin(char *file_name);

//-- MAIN ---------------------------------------------------------------------

int main(int argc, char **argv) {

  // Checking for an invalid number of arguments
  if (argc != 3) {
    printf("Expecting two arguments\n");
    return EXIT_FAILURE;
  }

  // Performing the first pass over the file
  read_ASM(argv[0]);

  // Perrforming the second pass over the file

  // Creating output binary file
  write_bin(argv[1]);
  

  return EXIT_SUCCESS;
}

void read_ASM(char *source_code) {

  // Open source assembly file
  FILE *fptr = fopen(source_code, "r");

  // Check to ensure that file exists
  if (fptr == NULL) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  // Store the length of the file and reset the pointer
  fseek(fptr, 0, SEEK_END);
  int file_size = ftell(fptr);
  rewind(fptr);

  // Convert bytes to words
  file_size /= 4;

  // Store the file contents in a structure yet to be defined

}

void write_bin(char *file_name) {
  
  // Creating output binary file
  FILE *fptr = fopen(file_name, "w+b");

  // Write to output file
  for (int i = 0; i < ; i++) {
    fwrite(, 32, 1, fptr);  
  }
}
