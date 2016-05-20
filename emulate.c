#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  FILE *fptr = fopen(argv[1], "rb");
  fseek(fptr, 0, SEEK_END);     // setting file pointer to last place
  int file_size = ftell(fptr);  // ftell returns the pos of the file pointer
  rewind(fptr);           // rewind resets the file pointer to the start pos
  unsigned char buffer[file_size];

  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }

  if (!fptr) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  fread(buffer, sizeof(buffer), 1, fptr);
  for (int i = 0; i < sizeof(buffer); i++) {
    // printf omits leading zeroes by default. 02 in %02x fixes this.
    printf("%02x", buffer[i]);
  }
  printf("\n");
  
  fclose(fptr);

  return EXIT_SUCCESS;

}
