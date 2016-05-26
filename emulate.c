#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMgen.h"
#include "bitOper.h"

CurrentState *currState = NULL;

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

  void updateCarry(int carry){
    
      currState->CPSR[29] = carry;
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

int32_t * getImmVal(int32_t * inst) {
  
  static int32_t value[32];
  int32_t rot_num_arry[4];
  int rot_num = 0;

  for(int i = 0; i < 32; i++) {
    value[i] = 0;
  }
  for(int i = 0; i < 8; i++) {
    value[31-i] = *(inst + i);
  } 

  for(int i = 8; i < 12; i++) {
    rot_num_arry[3 - (i - 8)] = *(inst + i);
  } 
  
  rot_num = convBinToDec(rot_num_arry, 4);


  rotate_right(value, sizeof(value), rot_num);

  return value;
}

int32_t * selectShift(int32_t * reg, int sbit1, int sbit2, int sft_num) {
  if (sbit1 == 0 && sbit2 == 0) {
     shift_left(reg, 32, sft_num);
  } else if (sbit1 == 0 && sbit2 == 1) {
     shift_right(reg, 32, sft_num);
  } else if (sbit1 == 1 && sbit2 == 0) {
     arith_shift_right(reg, 32, sft_num);
  } else if (sbit1 == 1 && sbit2 == 1) {
     rotate_right(reg, 32, sft_num);
  }
  return reg;
}

int32_t * getRegVal(int32_t * inst) {
  // Bits to select shifter type
  int32_t sbit1 = *(inst + 6), sbit2 = *(inst + 5);
  
  // Value at Rm
  int32_t *value;
  int32_t reg[4];
  for (int i = 3; i >= 0; i--) {
    reg[3-i] = *(inst + i);
  }
  value = *(currState->registers + convBinToDec(reg,4));
  
  // Using 5-bit integer
  if (*(inst + 4) == 0) {
    int32_t amount[5];
    for (int i = 11; i >= 7; i--) {
      amount[11 - i] = inst[i];
    }   
    return selectShift(value, sbit1, sbit2, convBinToDec(amount, 5));
  }  
  // Using register Rs
  if (*(inst + 4) == 1) {
    int32_t reg[4];
    for (int i = 11; i >= 8; i--) {
      reg[11 - i] = inst[i];
    }
    
    // Pointer to content in Register Rs
    int* regContent = *(currState->registers + convBinToDec(reg, 4));
    
    // Last byte of Rs register's content
    int32_t amount[8];
    for (int i = 7; i >= 0; i--) {
      amount[7 - i] = *(regContent + i);
    }
    return selectShift(value, sbit1, sbit2, convBinToDec(amount, 8));
  }

  return NULL;
}

int32_t * get_operand2(int32_t * inst) {
  int32_t *operand2 = NULL;
  operand2 = (*(inst + 25) == 1) ? getImmVal(inst) : getRegVal(inst);
  return operand2;
}

void copyArray(int32_t from[], int32_t to[], int size) {
  for(int i = 0; i < size; i++) {
    to[i] = from[i];
  }
  if(from != NULL) {
   free(from);
  }
}

void dataprocessing(int32_t * inst) {
// printf("Data processing was called!\n"); 
 int32_t rnarr[4], rdarr[4], opcodearr[4];
 int32_t *rn , *rd, *op2 = NULL;
 int32_t opcode = 0;
 int32_t rdVal[32];

 // Getting all nescessary components:
 //Getting operand2
 op2 = get_operand2(inst);

 // Getting registers Rn and Rd and opcode
 for (int i = 0; i < 4; i++) {
     rnarr[i] = inst[19 - i];
     rdarr[i] = inst[15 - i];
     opcodearr[i] = inst[24-i];
   } 

 rn = *(currState->registers+convBinToDec(rnarr, 4));
 rd = *(currState->registers+convBinToDec(rdarr, 4));
 opcode =convBinToDec(opcodearr, 4);  
 
 // Checking what the operation is, then executing it, and setting flags
 switch(opcode) {
  // Bitwise AND 
  case  0:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 1) && (*(op2 + i) == 1)) {
              *(rd + i) = 1;
              rdVal[i] = 1;
            } else {
              *(rd + i) = 0;
              rdVal[i] = 0;
            }
          }
          break;
  // Bitwise XOR
  case  1:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 0) && (*(op2 + i) == 1)) {
              *(rd + i) = 1;
              rdVal[i] = 1;
            } else  if ((*(rn + i) == 1) && (*(op2 + i) == 0)) {
              *(rd + i) = 1; 
              rdVal[i] = 1;
            } else {
              *(rd + i) = 0;
              rdVal[i] = 0;
            }
          }
          break;
  // SUB
  case  2:copyArray(binary_sub(rn, op2, 32), rdVal, 32); 
          for (int i = 0; i < 32; i++) {
             *(rd + i) = *(rdVal +i);
          }
          break;
  // RSB        
  case  3:copyArray(binary_sub(op2, rn, 32), rdVal,  32);
          for (int i = 0; i < 32; i++) {
              *(rd + i) = *(rdVal +i);
          }
          break;
  // ADD
  case  4:copyArray(binary_add(rn, op2, 32), rdVal, 32);
          for (int i = 0; i < 32; i++) {
              *(rd + i) = *(rdVal +i);
          }
          break;
  // TST
  case  8:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 1) && (*(op2 + i) == 1)) {
              rdVal[i] = 1;
            } else {
              rdVal[i] = 0;
            }
          }
          break;
  // TEQ
  case  9:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 0) && (*(op2 + i) == 1)) {
              rdVal[i] = 1;
            } else  if ((*(rn + i) == 1) && (*(op2 + i) == 0)) {
              rdVal[i] = 1;
            } else {
              rdVal[i] = 0;
            }
          }
          break;
  //CMP
  case 10:copyArray(binary_sub(rn, op2, 32), rdVal, 32);
          break;
  //ORR
  case 12:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 0) && (*(op2 + i) == 0)) {
              *(rd + i) = 0;
              rdVal[i] = 0;
            } else {
              *(rd + i) = 1;
              rdVal[i] = 1;
            }
          }
          break;
  //MOV
  case 13://printf("%d\n",convBinToDec((int32_t *) (op2),32));
          for (int i = 0; i < 32; i++) {
            *(rd+i) = *(op2+i);
            rdVal[i] = *(op2+i);
          }
          break;
  default: perror("Invalid OpCode\n"); return;

 }

 //set flags based on rdVal
 
 // V does not need to be set.
 // Setting c:
  
 

 // Setting z:
 int firstnum = 0;
 while ((firstnum < 32) && (*(rdVal + firstnum) == 0)) {
   firstnum ++;
 }
 if (firstnum == 32) {
   currState->CPSR[30] = 1;
 } else {
   currState->CPSR[30] = 0;
 }
 // Setting n:
 currState->CPSR[31] = *(rd + 0);

}


void multiply(int32_t * inst) {
  //Rd = Rm x Rs
  //Rd = Rm x Rs + Rn if A = 1
//  printf("Multiply gets called!\n");
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
  
  // updatng CPSR register
  if(inst[20] == 1){
    if(rd < 0){
      currState->CPSR[31] = 1;
      currState->CPSR[30] = 0;
    } else if(rd == 0){
      currState->CPSR[31] = 0;
      currState->CPSR[30] = 1;
    }
  }
}



void single_data_transfer(int32_t * inst) {
//  int32_t i = *(inst + 25);
//  printf("Data transfer gets called!\n");
  int32_t p = *(inst + 24);  
  int32_t u = *(inst + 23);
  int32_t l = *(inst + 20);
  int32_t rdarr[4], rnarr[4];
  
 
  for (int i = 0; i < 4; i++) {
    rdarr[i] = inst[15 - i];
    rnarr[i] = inst[19 - i];
  } 
  
  //if Rm != Rd

  int32_t *offset;
  if(*(inst + 25) == 0) {
    offset = getImmVal(inst);
  } else {
    offset = getRegVal(inst);
  }
  int32_t offsetVal = convBinToDec(offset, 32);
  
  int32_t rn = convBinToDec(*(currState->registers+convBinToDec(rnarr, 4)),32);
  
  if(l == 1) {
    if(p == 1) {
      *(currState->registers+convBinToDec(rdarr, 4)) 
        = convDecToBin(*(currState->memory + rn), 8);
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
    } else {
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
      *(currState->registers+convBinToDec(rdarr, 4)) 
        = convDecToBin(*(currState->memory + rn), 8);
    }
  } else {
    if(p == 1) {
      *(currState->memory + rn) 
        = convBinToDec(*(currState->registers+convBinToDec(rdarr, 4)),32);
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
    } else {
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
      *(currState->memory + rn) 
        = convBinToDec(*(currState->registers+convBinToDec(rdarr, 4)),32);
    }
  }
}

void branch(int32_t * inst) {
//  printf("Branch gets called!\n");
  int32_t offset[32];
  int offset_temp[24];
  for(int i = 0; i < 24; i++) {
    offset_temp[i] = *(inst + i);
  } 
  
//  printf("%d\n", convBinToDec(inst, 32));
//  printf("%d\n", convBinToDec(offset_temp, 24));

  //Shift left by 2
  shift_left(offset_temp, sizeof(offset_temp) / INSTRUCTION_BYTE_SIZE, 2);

  //Signed extended to 32 bits
  int sign = offset_temp[23];
  for(int i = 0; i < 32; i++) {
    if(i < 23) {
      offset[i] = offset_temp[i];
    } else {
      offset[i] = sign;
    }
  }
//  printf("%d\n", convBinToDec(offset, 32));
  //offset is a  SIGNED 32 bit bin number
  //Add offset to PC 
  currState->PC += convBinToDec(offset, 32) + 4;
  currState->pipeline->fetched = NULL;
  currState->pipeline->decoded = NULL;
  //Keeping the pipeline in mind, PC is 8 bytes ahead of instr)
}

// Temporary function for testing
void printBits(int inst) {
  uint32_t mask = 1 << 31;
  
  for (int i = 31; i >= 0; i--) {
    printf(((inst & mask) == 0) ? "0" : "1");
    inst <<= 1;
  }
  printf("\n"); 
}

void decode(int32_t * inst) {
  // set flags
  int32_t cond = 0,  goahead = 0 ;
  for (int i = 31; i >= 28; i--) {
    cond = cond * 2 + inst[i];
  }
  switch(cond) {
    case 0:  goahead = (currState->CPSR[30] == 1); break; 
    case 1:  goahead = (currState->CPSR[30] == 0); break;
    case 10: goahead = (currState->CPSR[31] == currState->CPSR[28]); break;
    case 11: goahead = (currState->CPSR[31] != currState->CPSR[28]); break;
    case 12: goahead = (currState->CPSR[30] == 0) && 
                       (currState->CPSR[31] == currState->CPSR[28]); break;
    case 13: goahead = (currState->CPSR[30] == 1) || 
                       (currState->CPSR[31] != currState->CPSR[28]); break;
    case 14: goahead = 1; break;
    default: perror("Invalid flags\n"); return;
  }

  if(!goahead) {return;}

  currState->pipeline->decoded = inst;   

  // subfunction distributor
  int32_t bit1 = *(inst + 27), bit2 = *(inst + 26);  
//  printf("Bit 27 = %d\n Bit 26 = %d\n", bit1, bit2);

//  printBits(convBinToDec(inst, 32));
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

  currState = calloc(1,sizeof(CurrentState));
  currState->pipeline = calloc(1,sizeof(Pipeline));

  if (currState == NULL || currState->pipeline == NULL) {
      perror("coudn't initialize state");
      exit(EXIT_FAILURE);
    }  

  for(int i=0; i < GEN_PURPOSE_REG; i++){  
    *(currState->registers + i) = (int32_t *)calloc(32 , sizeof(int32_t));

    if (*(currState->registers+i) == NULL) {
      perror("coudn't initialize registers");
      exit(EXIT_FAILURE);
    } 
  } 

  currState->PC = 0;
  currState->CPSR = (int32_t *) calloc(32, sizeof(int32_t));

  if (currState->CPSR == NULL) {
    perror("coudn't initialize registers");	 
    exit(EXIT_FAILURE);
  }

  currState->memory = calloc(MEMORY_CAPACITY, sizeof(int8_t));
  
  if (currState->memory == NULL) {
    perror("coudn't initialize memory");	 
    exit(EXIT_FAILURE);
  }

}  

//free memory
void freeRegs(){
  for(int i = 0; i < GEN_PURPOSE_REG; i++){
    free(*(currState->registers + i));
  }

  free(currState->CPSR);
  free(currState->pipeline);
  free(currState->memory);
  free(currState);
}

int allZeroes(int32_t* inst) {
  for (int i = 0; i < 32; i++) {
    if (*(inst + i) == 1) {
      return 0;
    }
  }
  return 1;
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


  //int8_t littleEndianBuffer[file_size]; // store instruction
  fread(currState->memory, sizeof(int8_t), file_size, fptr);  

  // fetch the instruction and store in byte
  int8_t* byte; 
 
  while (1) {
    byte = fetchInstruction(currState->memory);
    currState->pipeline->fetched = instrToBits(byte);
//    for (int i = 31; i >= 0; i--){
//    printf("%d", *(currState->pipeline->fetched + i));}
//    printf("\n");
//    printf("While loop called.\n");
    if (allZeroes(currState->pipeline->fetched) == 1) {
      free(byte);
      currState->PC += 4;
      break;
    }
    if (byte != NULL) {
      free(byte);
    }                     // free instruction memory
    decode(currState->pipeline->fetched);
  }    

  //Outputting the state of Registers and non-zero memory
  
  printf("Registers:\n");

  for(int i = 0; i <= 12; i++) {
    int reg_value = convBinToDec(*(currState->registers + i), 32);
    printf("$%-3d: %10d (0x%08x)\n", i,reg_value,reg_value);
  }

  printf("PC  : %10d (0x%08x)\n", currState->PC, currState->PC);
  printf("CPSR: %10d (0x%08x)\n", convBinToDec(currState->CPSR, 32),convBinToDec(currState->CPSR, 32));


  printf("Non-zero memory:\n");

  for (int i = 0; i < MEMORY_CAPACITY; i+=4) {
    int8_t *membyte = currState->memory + i;
    if (!(*membyte == 0) || !(*(membyte+1) == 0) || !(*(membyte+2) == 0) || !(*(membyte+3) == 0)){
      printf("0x%08x: ", i);
      printf("0x%02hhx%02hhx%02hhx%02hhx\n",(*membyte),(*(membyte+1)),(*(membyte+2)),*(membyte+3));
    } 
  }
  
  fclose(fptr);                  // close file

  freeRegs();                    // free memory

  return EXIT_SUCCESS;
}
