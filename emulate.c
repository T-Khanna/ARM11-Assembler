#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define INSTRUCTION_BYTE_SIZE 4

int pc = 0;

char * fetchInstruction(unsigned char littleEndianBuffer[]) {
  char* instruction = (char*) malloc(INSTRUCTION_BYTE_SIZE * sizeof(char));
  //static char* instruction;
  for (int i = pc; i < pc + INSTRUCTION_BYTE_SIZE; i++) {
    instruction[INSTRUCTION_BYTE_SIZE - (i - pc + 1)] = littleEndianBuffer[i];
  }  
  pc += 4;
  return instruction;
  // Need to 'free' memory occupied by instruction
}

int * printBits(char instruction[]) {
  unsigned char mask = 1 << 7;
  static int bininstruction[INSTRUCTION_BYTE_SIZE * 8];
  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    char byte = instruction[i];
    for (int j = 0; j < 8; j++) {
//      printf(((mask & byte) == 0) ? "0" : "1");
      bininstruction[31 - (i * 8 + j)] = ((mask & byte) == 0) ? 0 : 1;
      byte <<= 1;
    }
  }
  return bininstruction; 
}

void dataprocessing(int * inst) {

}

void multiply(int * inst) {

}

void datatransfer(int * inst) {

}

void branch(int * inst) {
  
}

void decode(int * inst) {
  int bit1 = *(inst + 27), bit2 = *(inst + 26);  
  if (bit1 == 1 && bit2 == 0) {
    branch(inst);
  } else if (bit1 == 0 && bit2 == 1) {
    datatransfer(inst);
  } else if (bit1 == 0 && bit2 == 0) {
    if (*(inst + 25) == 1) {
      dataprocessing(inst);
      return;
    } 
    if (*(inst + 4) == 0) {
      dataprocessing(inst);
      return;
    }
    if (*(inst + 7) == 0) {
      dataprocessing(inst);
    } else {
      multiply(inst);
    }
  } else {
    perror("Invalid instruction.\n");
  }
}

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }


  FILE *fptr = fopen(argv[1], "rb");

  if (!fptr) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  fseek(fptr, 0, SEEK_END);     // setting file pointer to last place
  int file_size = ftell(fptr);  // ftell returns the pos of the file pointer
  rewind(fptr);           // rewind resets the file pointer to the start pos
  unsigned char littleEndianBuffer[file_size];
  fread(littleEndianBuffer, sizeof(littleEndianBuffer), 1, fptr);
  char* str = fetchInstruction(littleEndianBuffer);
/*  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    // printf omits leading zeroes by default. 02 in %02x fixes this.
    printf("%02x\n", (unsigned char) *(str + i));
  } */ 
/*  for(int i = 31; i >= 0; i--) {
    printf("%d",*(ints+i));
  }
  printf("\n");*/
  int *ints = printBits(str);
  free(str);
  decode(ints);
  fclose(fptr);
  return EXIT_SUCCESS;
}
