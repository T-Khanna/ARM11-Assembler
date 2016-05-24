#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMgen.h"
#define INSTRUCTION_BYTE_SIZE 4

int32_t pc = 0;
int32_t** registers = NULL;

int8_t * fetchInstruction(int8_t littleEndianBuffer[]) {
int8_t* instruction = (int8_t*) malloc(INSTRUCTION_BYTE_SIZE * sizeof(int8_t));
  //static int32_t8_t* instruction;
  for (int32_t i = pc; i < pc + INSTRUCTION_BYTE_SIZE; i++) {
    instruction[INSTRUCTION_BYTE_SIZE - (i - pc + 1)] = littleEndianBuffer[i];
  }  
  pc += 4;
  return instruction;
  // Need to 'free' memory occupied by instruction
}

int32_t* instrToBits(int8_t instruction[]) {
// retrun int32_t array in binary form from instruction int32_t8_t array
  int8_t mask = 1 << 7;
  static int32_t bininstruction[INSTRUCTION_BYTE_SIZE * 8];
  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    int8_t byte = instruction[i];
    for (int j = 0; j < 8; j++) {
// print32_tf(((mask & byte) == 0) ? "0" : "1");
      bininstruction[31 - (i * 8 + j)] = ((mask & byte) == 0) ? 0 : 1;
      byte <<= 1;
    }
  }
  return bininstruction; 
}

void dataprocessing(int32_t * inst) {

}

int32_t convBinToDec(int32_t bin[], int32_t size) {
// converts binary array to int32_t
  static int32_t result = 0;
  for (int32_t i = 0; i < size; i++) {
    result = result * 2 + bin[i];
  }
  return result;
}

int32_t* convDecToBin(int32_t num, int32_t size){ 
// convert int32_t to binary array
  static int32_t* result = NULL;
  for (int i =0; i < size; i++){
    result[size - i] = (num % 2);
    num = num / 2; 
  }
  return result;
}


void multiply(int32_t * inst) {
  //Rd = Rm x Rs
  //Rd = Rm x Rs + Rn if A = 1
  int32_t rdarr[4], rmarr[4], rsarr[4], rnarr[4];
  for (int i = 0; i < 4; i++) {
    rdarr[i] = inst[19 - i];
    rmarr[i] = inst[3 - i];
    rsarr[i] = inst[11 - i];
    rnarr[i] = inst[15 - i];
  } 
  int32_t rd = convBinToDec(registers[convBinToDec(rdarr, 4)],32);  //calculate value of rd
  int32_t rm = convBinToDec(registers[convBinToDec(rmarr, 4)],32);  //calculate value of rm
  int32_t rs = convBinToDec(registers[convBinToDec(rsarr, 4)],32);  //calculate value of rs
  
  // Checking and adding Rn
  if(inst[21] == 1) {
    int32_t rn = convBinToDec(registers[convBinToDec(rnarr, 4)],32);  //calculate value of rn
    rd = rm * rs + rn;
    //Add Rn
  } else {
    rd = rm * rs;
    // Not adding Rn
  }

  // saving result in register 
  registers[convBinToDec(rdarr, 4)] = convDecToBin(rd,32);
  
  // updatng CPRS register
  if(inst[20] == 1){
    if(rd < 0){
      registers[16][31] = 1;
      registers[16][30] = 0;
      registers[16][29] = 0;
      registers[16][28] = 0;
    } else if(rd ==0){
      registers[16][31] = 0;
      registers[16][30] = 1;
      registers[16][29] = 0;
      registers[16][28] = 0;
    }
  }
}

void datatransfer(int32_t * inst) {

}

void branch(int32_t * inst) {
  
}

void decode(int32_t * inst) {
  // set flags
  int32_t cond = 0,  goahead = 0 ;
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
  int32_t bit1 = *(inst + 27), bit2 = *(inst + 26);  
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

int32_t ** regInit(){
  int32_t** registers = (int32_t **) malloc(17 * sizeof(int32_t*));
  
  for(int i=0; i < 17; i++){
    registers[i] = (int32_t *) malloc(32 * sizeof(int32_t));
    for(int j = 0; j < 32; j++){
      registers[i][j] = 0;
    }
  }
  return registers; 
  //free memory
}  

void freeRegs(int32_t **registers){
  for(int32_t i = 0; i < 17; i++){
    free(registers[i]);
  }
  free(registers);
}

int32_t main(int32_t argc, char **argv) {

  // checking arguments
  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }
 
  // initialize registers
  registers = regInit();

  // open file
  FILE *fptr = fopen(argv[1], "rb");
 
  // pass error if unvalid file
  if (!fptr) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  fseek(fptr, 0, SEEK_END);     // setting file point32_ter to last place
  int32_t file_size = ftell(fptr);  // ftell returns the pos of the file point32_ter
  rewind(fptr);           // rewind resets the file point32_ter to the start pos

  int8_t littleEndianBuffer[file_size]; // store instruction
  fread(littleEndianBuffer, sizeof(littleEndianBuffer), 1, fptr);  

  // --- loop begin ----

  // fetch the instruction and store in str
  int8_t* str = fetchInstruction(littleEndianBuffer);


/*  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    // printf omits leading zeroes by default. 02 in %02x fixes this.
    printf("%02x\n", (unsigned int8_t) *(str + i));
  } */ 
/*  for(inti = 31; i >= 0; i--) {
    printf("%d",*(ints+i));
  }
  printf("\n");*/


  int32_t *ints = instrToBits(str);    
  free(str);                     // free instruction memory
  decode(ints);    


  // ---- loop end ----
  fclose(fptr);                  // close file
  freeRegs(registers);           // free memory
  return EXIT_SUCCESS;
}
