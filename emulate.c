#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMgen.h"
#include "bitOper.h"


CurrentState *currState;

int8_t * fetchInstruction(int8_t littleEndianBuffer[]) {

   int8_t* instruction = (int8_t*) calloc(INSTRUCTION_BYTE_SIZE, sizeof(int8_t));

   //checking for instruction memory
   if (instruction == NULL) {
      perror("coudn't initialize instructions");
      exit(EXIT_FAILURE);
   }


  //static int32_t8_t* instruction;
  for (int i = currState->PC; i < currState->PC + INSTRUCTION_BYTE_SIZE; i++) {
    instruction[INSTRUCTION_BYTE_SIZE - (i - currState->PC + 1)] = littleEndianBuffer[i];
  }  
  currState->PC += 4;
  return instruction;
  // Need to 'free' memory occupied by instruction
}

int32_t* instrToBits(int8_t instruction[]) {
// retrun int32_t array in binary form from instruction int32_t8_t array
  uint8_t mask = 1 << 7;
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
  int32_t rd = convBinToDec(*(currState->registers+convBinToDec(rdarr, 4)),32);  //calculate value of rd
  int32_t rm = convBinToDec(*(currState->registers+convBinToDec(rmarr, 4)),32);  //calculate value of rm
  int32_t rs = convBinToDec(*(currState->registers+convBinToDec(rsarr, 4)),32);  //calculate value of rs
  
  // Checking and adding Rn
  if(inst[21] == 1) {
    int32_t rn = convBinToDec(*(currState->registers+convBinToDec(rnarr, 4)),32);  //calculate value of rn
    rd = rm * rs + rn;
    //Add Rn
  } else {
    rd = rm * rs;
    // Not adding Rn
  }

  // saving result in register 
  *(currState->registers+convBinToDec(rdarr, 4)) = convDecToBin(rd,32);
  
  // updatng CPRS register
  if(inst[20] == 1){
    if(rd < 0){
      currState->CPRS[31] = 1;
      currState->CPRS[30] = 0;
    } else if(rd == 0){
      currState->CPRS[31] = 0;
      currState->CPRS[30] = 1;
    }
  }
}


int32_t * getImmVal(int32_t * inst) {
  static int32_t *value;
  static int32_t *rot_num_arry;
  int rot_num = 0;

  for(int i = 0; i < 32; i++) {
    *(value + i) = 0;
    *(rot_num_arry + i) = 0;
  } 

  for(int i = 0; i < 8; i++) {
    *(value + i) = *(inst + i);
  } 

  for(int i = 8; i < 12; i++) {
    *(rot_num_arry + (i - 8)) = *(inst + i);
  } 

  rot_num = convBinToDec(rot_num_arry, 4);

  rotate_right(value, sizeof(value), rot_num);

  return value;
}

int32_t * getRegVal(int32_t * inst) {
  
  return NULL;
}

int32_t * get_operand2(int32_t * inst) {
  static int32_t *operand2;

  if(*(inst + 25) == 1) {
    operand2 = getImmVal(inst);
  } else {
    operand2 = getRegVal(inst);
  }

  return operand2;
}


void single_data_transfer(int32_t * inst) {

}

/*
void single_data_transfer(int32_t * inst) {
  int32_t i = *(inst + 25);
  int32_t p = *(inst + 24);
  int32_t u = *(inst + 23);
  int32_t l = *(inst + 20);
  int32_t rdarr[4], rnarr[4], offset[12];
  
 
  for (int i = 0; i < 4; i++) {
    rdarr[i] = inst[15 - i];
    rnarr[i] = inst[19 - i];
  } 
  
  for (int i = 0; i < 12; i++) {
    offset[i] = inst[11 - i];
  }

}

*/


void branch(int32_t * inst) {
  int32_t offset;
  
  //Shift left by 2
  //Signed extended to 32 bits
  //Add offset to PC 
  //(Keep the pipeline in mind though - PC is 8 bytes ahead of instr)

}

void decode(int32_t * inst) {
  // set flags
  int32_t cond = 0,  goahead = 0 ;
  for (int i = 31; i >= 28; i--) {
    cond = cond * 2 + inst[i];
  }
  switch(cond) {
    case 0:  goahead = (currState->CPRS[30] == 1); break; 
    case 1:  goahead = (currState->CPRS[30] == 0); break;
    case 10: goahead = (currState->CPRS[31] == currState->CPRS[28]); break;
    case 11: goahead = (currState->CPRS[31] != currState->CPRS[28]); break;
    case 12: goahead = (currState->CPRS[30] == 0) && 
                       (currState->CPRS[31] == currState->CPRS[28]); break;
    case 13: goahead = (currState->CPRS[30] == 1) || 
                       (currState->CPRS[31] != currState->CPRS[28]); break;
    case 14: goahead = 1; break;
    default: perror("Invalid flags\n"); return;
  }

  if(!goahead) {return;}

  currState->pipeline->decoded = inst;

  // subfunction distributor
  int32_t bit1 = *(inst + 27), bit2 = *(inst + 26);  
  if (bit1 == 1 && bit2 == 0) {
    branch(inst);
  } else if (bit1 == 0 && bit2 == 1) {
    single_data_transfer(inst);
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

//initialize all registers
void regInit(){
  for(int i=0; i < GEN_PURPOSE_REG; i++){
    *(currState->registers + i) = (int32_t *) calloc(32, sizeof(int32_t));
  
   if (*(currState->registers+i) == NULL) {
    perror("coudn't initialize registers");
    exit(EXIT_FAILURE);
  } 
}

  currState->PC = 0;
  currState->CPRS = (int32_t *) calloc(32, sizeof(int32_t));

  if (currState->CPRS == NULL) {
    perror("coudn't initialize registers");	 
    exit(EXIT_FAILURE);
  }
}  

//free memory
void freeRegs(){
  for(int i = 0; i < 13; i++){
    free(*(currState->registers + i));
  }
  free(currState->CPRS);
}

int32_t main(int32_t argc, char **argv) {

  // checking arguments
  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }
 
  // initialize registers
  regInit();

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

  // fetch the instruction and store in byte
  int8_t* byte = fetchInstruction(littleEndianBuffer);


/*  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    // printf omits leading zeroes by default. 02 in %02x fixes this.
    printf("%02x\n", (unsigned int8_t) *(str + i));
  } */ 
/*  for(inti = 31; i >= 0; i--) {
    printf("%d",*(ints+i));
  }
  printf("\n");*/


  currState->pipeline->fetched = instrToBits(byte);    
  if (byte != NULL){
  free(byte);}                     // free instruction memory
  decode(currState->pipeline->fetched);    


  // ---- loop end ----
  fclose(fptr);                  // close file
  freeRegs();                    // free memory
  return EXIT_SUCCESS;
}
