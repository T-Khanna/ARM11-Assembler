#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define INSTRUCTION_BYTE_SIZE 4

int pc = 0;
int** registers = NULL;

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

int convBinToDec(int bin[], int size) {
  static int result = 0;
  for (int i = 0; i < size; i++) {
    result = result * 2 + bin[i];
  }
  return result;
}


void multiply(int * inst) {
  //Rd = Rm x Rs
  //Rd = Rm x Rs + Rn if A = 1
  int rdarr[4], rmarr[4], rsarr[4], rnarr[4];
  for (int i = 0; i < 4; i++) {
    rdarr[i] = inst[19 - i];
    rmarr[i] = inst[3 - i];
    rsarr[i] = inst[11 - i];
    rnarr[i] = inst[15 - i];
  } 
  int rd = convBinToDec(rdarr, 4);
  int rm = convBinToDec(rmarr, 4);
  int rs = convBinToDec(rsarr, 4);
  
  if(inst[21] == 1) {
  int rn = convBinToDec(rnarr, 4);
    //Add Rn
  }
  
  //Make sure you set the flags here depending on S
}

void datatransfer(int * inst) {

}

void branch(int * inst) {
  
}

void decode(int * inst) {
  // set flags
  int cond = 0,  goahead = 0 ;
  for (int i = 31; i >= 28; i--) {
    cond = cond * 2 + inst[i];
  }
  switch(cond) {
    case 0:  goahead = (registers[16][30] == 1); break; 
    case 1:  goahead = (registers[16][30] == 0); break;
    case 10: goahead = (registers[16][31] == registers[16][28]); break;
    case 11: goahead = (registers[16][31] != registers[16][28]); break;
    case 12: goahead = (registers[16][30] == 0) && 
                       (registers[16][31] == registers[16][28]); break;
    case 13: goahead = (registers[16][30] == 1) || 
                       (registers[16][31] != registers[16][28]); break;
    case 14: goahead = 1; break;
    default: perror("Invalid flags\n"); return;
  }

  if(!goahead) {return;}

  // subfunction distributor
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

int ** regInit(){
  int** registers = (int **) malloc(17 * sizeof(int*));
  
  for(int i=0; i < 17; i++){
    registers[i] = (int *) malloc(32 * sizeof(int));
    for(int j = 0; j < 32; j++){
      registers[i][j] = 0;
    }
  }
  return registers; 
  //free memory
}  

void freeRegs(int **registers){
  for(int i = 0; i < 17; i++){
    free(registers[i]);
  }
  free(registers);
}

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }

  int **registers = regInit();

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
  freeRegs(registers);
  return EXIT_SUCCESS;
}
