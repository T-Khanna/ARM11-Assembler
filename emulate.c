#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*void printbits(int num) {
  uint32_t mask = 1 << 31;

  for (int i = 0; i < 32; i++) {
    printf(((num & mask) == 0) ? "0" : "1");
    num <<= 1;
  }
  printf("\n");
}*/

int main(int argc, char **argv) {

  unsigned char buffer[10];
  FILE *fptr;

  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }

  fptr = fopen(argv[1], "rb");

  if (!fptr) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  fread(buffer, sizeof(buffer), 1, fptr);
  for (int i = 0; i < sizeof(buffer); i++) {
    // printbits(buffer[i]);
    printf("%x", buffer[i]);
  }
  printf("\n");
  
  fclose(fptr);

  return EXIT_SUCCESS;

}
