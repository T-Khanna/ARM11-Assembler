///////////////////////////////////////////////////////////////////////////////
//ARM Group Project - Year 1 (Group 40)
//____________________________________________________________________________
//
//File: emulate.c
//Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
///////////////////////////////////////////////////////////////////////////////


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMgen.h"
#include "bitOper.h"


//-- CONSTANTS ---------------------------------------------------------------- 

#define BYTE_SIZE 8 
#define PC registers[15] 
#define CPSR registers[16] 


//-- FUNCTION DECLARATIONS ----------------------------------------------------

void reg_init();
void print_output();
int conv_endian(int32_t num);
int check_condition(int32_t* intr);
int32_t decode(int32_t* intr);
void branch(int32_t* intr);
void data_processing(int32_t* intr);
void multiply(int32_t* intr);
void single_data_transfer(int32_t* intr);


//-- GLOBAL VARIABLES ---------------------------------------------------------

  //curr_state holds the state of registers, memory and the pipeline
  current_state curr_state;


//-- MAIN ---------------------------------------------------------------------

int32_t main(int argc, char *argv[]) {

  //checking for an invalid number of arguments
  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }

  //initialises the memory, pipeline and registers
  reg_init(argv[1]);

  //fetch, decode, execute cycle
  do {
    
    //fetch instruction
    curr_state.pipeline.fetched 
         = curr_state.memory[curr_state.PC];
    
    //increment the PC
    curr_state.PC ++;

    //checking condition (COND) for decoding and executing
    int goAhead 
         = check_condition(&curr_state.pipeline.fetched);
 
    //decode
    if (goAhead){

      //decode current fetched instruction in pipeline and store as the 
      //decoded instruction in the pipeline.
      curr_state.pipeline.decoded = decode(&curr_state.pipeline.fetched);

      //execute appropriate instruction depending on decoded instruction
      switch (curr_state.pipeline.decoded){

        case Single_data_transfer:
             single_data_transfer(&curr_state.pipeline.fetched);
             break;

        case Branch:
             branch(&curr_state.pipeline.fetched);
             break;

        case Data_processing:
             data_processing(&curr_state.pipeline.fetched);
             break;

        case Multiply:
             multiply(&curr_state.pipeline.fetched);
             break;

      }

    }

  } while(curr_state.pipeline.fetched != 0); //fetch, decode, execute cycle
 
  //print output of state of registers and non-zero memory.
  print_output();

  return EXIT_SUCCESS;
}


//-- SUPPORTING FUNCTIONS -----------------------------------------------------

//initialises registers and memory by setting them to 0.
void reg_init(char* file){
   
  //setting all registers to 0
  for (int i = 0; i < TOTAL_REGISTERS; i++) {
     curr_state.registers[i] = 0;
  }
  
  //clearing the pipeline
  curr_state.pipeline.fetched = 0;
  curr_state.pipeline.decoded = 0;

  //setting all memory to 0
  for (int i = 0; i < MEMORY_CAPACITY; i++) {
      curr_state.memory[i] = 0;
  } 

  //moving intructions into memory
  FILE *fptr = fopen(file,"rb");

  //passing an error if file is invalid
  if (fptr == NULL) {
     printf("Unable to open file\n");
     return;
  }

  //setting file point32_ter to the end
  fseek(fptr, 0, SEEK_END);         

  //ftell returns the pos of the file point32_ter
  int file_size = ftell(fptr);	
  rewind(fptr); 
  
  //Saving the file data in memory
  for (int i = 0; i < file_size; i++) {

    fread(&curr_state.memory[i], 32, 1, fptr);

    //converting instructions from little endian to big endian 
    curr_state.memory[i] = conv_endian(curr_state.memory[i]);

  }

  //closing the binary input file
  fclose(fptr);

}


//converts from one Endian to the other
int conv_endian(int num){

  int result = 0;

  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++){
        result = result | (getBits(&num, i * BYTE_SIZE, BYTE_SIZE)
                   << ((INSTRUCTION_BYTE_SIZE -1 - i) * BYTE_SIZE));
  }

  return result;

}


//printing state of registers and all non-zero memory
void print_output() {

  //printing state of general purpose registers
  printf("Registers:\n");
  for(int i = 0; i < GEN_PURPOSE_REG; i++) {
    int reg_value = curr_state.registers[i];
    printf("$%-3d: %10d (0x%08x)\n", i, reg_value, reg_value);
  }

  //printing state of PC and CPSR registers
  printf("PC  : %10d (0x%08x)\n", curr_state.PC, curr_state.PC);
  printf("CPSR: %10d (0x%08x)\n", curr_state.CPSR, curr_state.CPSR);

  //printing non-zero memory
  printf("Non-zero memory:\n");

  for (int i = 0; i < MEMORY_CAPACITY; i++) {

    int32_t memWord = curr_state.memory[i];
    
    if (!(memWord == 0)){  //checking for non-zero memory
      printf("0x%08x: ", 4*i);
      printf("0x%08x\n", curr_state.memory[i]);
    }       

  }

}

 
//checks condition (COND) to decode and execute the instructions 
//locations of flags: Nflag = 31, Zflag = 30, Cflag = 29, Vflag = 28
int check_condition(int32_t* intr){

   int case_num = getBits(intr, 28 , 4);
  
   switch(case_num){

     //check z flag is set
     case 0:  return getBit(&curr_state.CPSR,30);
      
     //check z flag is clear
     case 1:  if (getBit(&curr_state.CPSR,30) == 0){         
                 return 1;
              }

     //check n equal to v
     case 10: if (getBit(&curr_state.CPSR,31) 
                    == getBit(&curr_state.CPSR,28)){
                 return 1;
              } 

     //n not equal to v
     case 11: if (getBit(&curr_state.CPSR,31)
                    != getBit(&curr_state.CPSR,28)){
                 return 1;
              }

     //check z clear and n equal to v
     case 12: if ((getBit(&curr_state.CPSR,31)
                     == getBit(&curr_state.CPSR,28))
                     && getBit(&curr_state.CPSR,30) == 0){
                 return 1;
              }

     //check z or (n not equal to v)
     case 13: if ((getBit(&curr_state.CPSR,30)==1) 
                     || (getBit(&curr_state.CPSR,31)
                     != getBit(&curr_state.CPSR,28))){
                 return 1;
              }

     //all flags
     case 14: return 1;

     default: return 0;

   }
}


//decodes the intruction and returns it as a decoded integer, depending upon 
//the given conditions for the appropriate bits.
int32_t decode(int32_t* intr){

  //fetching bits 26 and 27
  int bit276 = getBit(intr,26);
  int bit27 = getBit(intr,27);  
  
  //if bit 26 and 27 are 0 then the instruction can be
  //Multiply or Data Processing
  if ((bit276==0) && (bit27==0)){
      
    //if I = 1 then the instruction is Data Processing 
    if (getBit(intr,25)){
         return Data_processing;
    }

    if (getBit(intr,4)){

      //if bit 4 and 7 are set then the instruction is Multiply
      if(getBit(intr,7)){
           return Multiply;}
      }

      //else it is Data Processing
      return Data_processing;

     }

  //if bit 26 is 1 then the instruction is Single Data Transfer 
  else if ((bit276==1) && (bit27==0)) {
    return Single_data_transfer;
  }

  //otherwise, if none of the above cases pass, the instruction is Branch
  else {
    return Branch;
  }

}


//-----------------------------------------------
////functions to be done. Can we split these up into different files please.
void branch(int32_t* intr){}
void data_processing(int32_t* intr){}
void multiply(int32_t* intr){}
void single_data_transfer(int32_t* intr){}

//-----------------------------------------------
/*
int8_t * fetchInstruction(int8_t littleEndianBuffer[]) {

   int8_t* instruction = (int8_t*) calloc(INSTRUCTION_BYTE_SIZE, sizeof(int8_t));

   //checking for instruction memory
   if (instruction == NULL) {
      perror("coudn't initialize instructions");
      exit(EXIT_FAILURE);
   }


  //static int32_t8_t* instruction;
  for (int i = curr_state->PC; i < curr_state->PC + INSTRUCTION_BYTE_SIZE; i++) {
    instruction[INSTRUCTION_BYTE_SIZE - (i - curr_state->PC + 1)] = littleEndianBuffer[i];
  }  
  curr_state->PC += 4;
  return instruction;
  //Need to 'free' memory occupied by instruction
}

  void updateCarry(int carry){
    
      curr_state->CPSR[29] = carry;
}

int32_t* instrToBits(int8_t instruction[]) {
//retrun int32_t array in binary form from instruction int32_t8_t array
  uint8_t mask = 1 << 7;
  static int32_t bininstruction[INSTRUCTION_BYTE_SIZE * 8];
  for (int i = 0; i < INSTRUCTION_BYTE_SIZE; i++) {
    int8_t byte = instruction[i];
    for (int j = 0; j < 8; j++) {
//print32_tf(((mask & byte) == 0) ? "0" : "1");
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

int32_t * selectShift(int32_t * reg, int sbit276, int sbit27, int sft_num) {
  if (sbit276 == 0 && sbit27 == 0) {
     shift_left(reg, 32, sft_num);
  } else if (sbit276 == 0 && sbit27 == 1) {
     shift_right(reg, 32, sft_num);
  } else if (sbit276 == 1 && sbit27 == 0) {
     arith_shift_right(reg, 32, sft_num);
  } else if (sbit276 == 1 && sbit27 == 1) {
     rotate_right(reg, 32, sft_num);
  }
  return reg;
}

int32_t * getRegVal(int32_t * inst) {
  //Bits to select shifter type
  int32_t sbit276 = *(inst + 6), sbit27 = *(inst + 5);
  
  //Value at Rm
  int32_t *value;
  int32_t reg[4];
  for (int i = 3; i >= 0; i--) {
    reg[3-i] = *(inst + i);
  }
  value = *(curr_state->registers + convBinToDec(reg,4));
  
  //Using 5-bit integer
  if (*(inst + 4) == 0) {
    int32_t amount[5];
    for (int i = 11; i >= 7; i--) {
      amount[11 - i] = inst[i];
    }   
    return selectShift(value, sbit276, sbit27, convBinToDec(amount, 5));
  }  
  //Using register Rs
  if (*(inst + 4) == 1) {
    int32_t reg[4];
    for (int i = 11; i >= 8; i--) {
      reg[11 - i] = inst[i];
    }
    
    //Pointer to content in Register Rs
    int* regContent = *(curr_state->registers + convBinToDec(reg, 4));
    
    //Last byte of Rs register's content
    int32_t amount[8];
    for (int i = 7; i >= 0; i--) {
      amount[7 - i] = *(regContent + i);
    }
    return selectShift(value, sbit276, sbit27, convBinToDec(amount, 8));
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

void data_processing(int32_t * inst) {
//printf("Data processing was called!\n"); 
 int32_t rnarr[4], rdarr[4], opcodearr[4];
 int32_t *rn , *rd, *op2 = NULL;
 int32_t opcode = 0;
 int32_t rdVal[32];

 //Getting all nescessary components:
 //Getting operand2
 op2 = get_operand2(inst);

 //Getting registers Rn and Rd and opcode
 for (int i = 0; i < 4; i++) {
     rnarr[i] = inst[19 - i];
     rdarr[i] = inst[15 - i];
     opcodearr[i] = inst[24-i];
   } 

 rn = *(curr_state->registers+convBinToDec(rnarr, 4));
 rd = *(curr_state->registers+convBinToDec(rdarr, 4));
 opcode =convBinToDec(opcodearr, 4);  
 
 //Checking what the operation is, then executing it, and setting flags
 switch(opcode) {
  //Bitwise AND 
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
  //Bitwise XOR
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
  //SUB
  case  2:copyArray(binary_sub(rn, op2, 32), rdVal, 32); 
          for (int i = 0; i < 32; i++) {
             *(rd + i) = *(rdVal +i);
          }
          break;
  //RSB        
  case  3:copyArray(binary_sub(op2, rn, 32), rdVal,  32);
          for (int i = 0; i < 32; i++) {
              *(rd + i) = *(rdVal +i);
          }
          break;
  //ADD
  case  4:copyArray(binary_add(rn, op2, 32), rdVal, 32);
          for (int i = 0; i < 32; i++) {
              *(rd + i) = *(rdVal +i);
          }
          break;
  //TST
  case  8:for (int i = 0; i < 32; i++) {
            if ((*(rn + i) == 1) && (*(op2 + i) == 1)) {
              rdVal[i] = 1;
            } else {
              rdVal[i] = 0;
            }
          }
          break;
  //TEQ
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
 
 //V does not need to be set.
 //Setting c:
  
 

 //Setting z:
 int firstnum = 0;
 while ((firstnum < 32) && (*(rdVal + firstnum) == 0)) {
   firstnum ++;
 }
 if (firstnum == 32) {
   curr_state->CPSR[30] = 1;
 } else {
   curr_state->CPSR[30] = 0;
 }
 //Setting n:
 curr_state->CPSR[31] = *(rd + 0);

}


void multiply(int32_t * inst) {
  //Rd = Rm x Rs
  //Rd = Rm x Rs + Rn if A = 1
//printf("Multiply gets called!\n");
  int32_t rdarr[4], rmarr[4], rsarr[4], rnarr[4];
  for (int i = 0; i < 4; i++) {
    rdarr[i] = inst[19 - i];
    rmarr[i] = inst[3 - i];
    rsarr[i] = inst[11 - i];
    rnarr[i] = inst[15 - i];
  } 
  int32_t rd = convBinToDec(*(curr_state->registers+convBinToDec(rdarr, 4)),32);  //calculate value of rd
  int32_t rm = convBinToDec(*(curr_state->registers+convBinToDec(rmarr, 4)),32);  //calculate value of rm
  int32_t rs = convBinToDec(*(curr_state->registers+convBinToDec(rsarr, 4)),32);  //calculate value of rs
  //Checking and adding Rn
  if(inst[21] == 1) {
    int32_t rn = convBinToDec(*(curr_state->registers+convBinToDec(rnarr, 4)),32);  //calculate value of rn
    rd = rm * rs + rn;
    //Add Rn
  } else {
    rd = rm * rs;
    //Not adding Rn
  }
  //saving result in register

  for (int i=0; i < 32; i++){
  *(*(curr_state->registers+convBinToDec(rdarr, 4)) + i) = *(convDecToBin(rd,32) + i);
  
  }

  //updatng CPSR register
  if(inst[20] == 1){
    if(rd < 0){
      curr_state->CPSR[31] = 1;
      curr_state->CPSR[30] = 0;
    } else if(rd == 0){
      curr_state->CPSR[31] = 0;
      curr_state->CPSR[30] = 1;
    }
  }
}



void single_data_transfer(int32_t * inst) {
//int32_t i = *(inst + 25);
//printf("Data transfer gets called!\n");
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
  
  int32_t rn = convBinToDec(*(curr_state->registers+convBinToDec(rnarr, 4)),32);
  
  if(l == 1) {
    if(p == 1) {
      *(curr_state->registers+convBinToDec(rdarr, 4)) 
        = convDecToBin(*(curr_state->memory + rn), 8);
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
    } else {
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
      *(curr_state->registers+convBinToDec(rdarr, 4)) 
        = convDecToBin(*(curr_state->memory + rn), 8);
    }
  } else {
    if(p == 1) {
      *(curr_state->memory + rn) 
        = convBinToDec(*(curr_state->registers+convBinToDec(rdarr, 4)),32);
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
    } else {
      (u == 1) ? (rn += offsetVal) : (rn -= offsetVal);
      *(curr_state->memory + rn) 
        = convBinToDec(*(curr_state->registers+convBinToDec(rdarr, 4)),32);
    }
  }
}

void branch(int32_t * inst) {
//printf("Branch gets called!\n");
  int32_t offset[32];
  int offset_temp[24];
  for(int i = 0; i < 24; i++) {
    offset_temp[i] = *(inst + i);
  } 
  
//printf("%d\n", convBinToDec(inst, 32));
//printf("%d\n", convBinToDec(offset_temp, 24));

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
//printf("%d\n", convBinToDec(offset, 32));
  //offset is a  SIGNED 32 bit bin number
  //Add offset to PC 
  curr_state->PC += convBinToDec(offset, 32) + 4;
  curr_state->pipeline->fetched = NULL;
  curr_state->pipeline->decoded = NULL;
  //Keeping the pipeline in mind, PC is 8 bytes ahead of instr)
}

//Temporary function for testing
void printBits(int inst) {
  uint32_t mask = 1 << 31;
  
  for (int i = 31; i >= 0; i--) {
    printf(((inst & mask) == 0) ? "0" : "1");
    inst <<= 1;
  }
  printf("\n"); 
}

void decode(int32_t * inst) {
  //set flags
  int32_t cond = 0,  goahead = 0 ;
  for (int i = 31; i >= 28; i--) {
    cond = cond * 2 + inst[i];
  }
  switch(cond) {
    case 0:  goahead = (curr_state->CPSR[30] == 1); break; 
    case 1:  goahead = (curr_state->CPSR[30] == 0); break;
    case 10: goahead = (curr_state->CPSR[31] == curr_state->CPSR[28]); break;
    case 11: goahead = (curr_state->CPSR[31] != curr_state->CPSR[28]); break;
    case 12: goahead = (curr_state->CPSR[30] == 0) && 
                       (curr_state->CPSR[31] == curr_state->CPSR[28]); break;
    case 13: goahead = (curr_state->CPSR[30] == 1) || 
                       (curr_state->CPSR[31] != curr_state->CPSR[28]); break;
    case 14: goahead = 1; break;
    default: perror("Invalid flags\n"); return;
  }

  if(!goahead) {return;}

  curr_state->pipeline->decoded = inst;   

  //subfunction distributor
  int32_t bit276 = *(inst + 27), bit27 = *(inst + 26);  
//printf("Bit 27 = %d\n Bit 26 = %d\n", bit276, bit27);

//printBits(convBinToDec(inst, 32));
  if (bit276 == 1 && bit27 == 0) {
    branch(inst);
  } else if (bit276 == 0 && bit27 == 1) {
    single_data_transfer(inst);
  } else if (bit276 == 0 && bit27 == 0) {
    if (*(inst + 25) == 1) {
      data_processing(inst);
      return;
    } 
    if (*(inst + 4) == 0) {
      data_processing(inst);
      return;
    }
    if (*(inst + 7) == 0) {
      data_processing(inst);
    } else {
      multiply(inst);
    }
  } else {
    perror("Invalid instruction.\n");
  }
}

//initialize all registers
void reg_init(){

  curr_state = calloc(1,sizeof(current_state));
  curr_state->pipeline = calloc(1,sizeof(pipeline));

  if (curr_state == NULL || curr_state->pipeline == NULL) {
      perror("coudn't initialize state");
      exit(EXIT_FAILURE);
    }  

  for(int i=0; i < GEN_PURPOSE_REG; i++){  
    *(curr_state->registers + i) = (int32_t *)calloc(32 , sizeof(int32_t));

    if (*(curr_state->registers+i) == NULL) {
      perror("coudn't initialize registers");
      exit(EXIT_FAILURE);
    } 
  } 

  curr_state->PC = 0;
  curr_state->CPSR = (int32_t *) calloc(32, sizeof(int32_t));

  if (curr_state->CPSR == NULL) {
    perror("coudn't initialize registers");	 
    exit(EXIT_FAILURE);
  }

  curr_state->memory = calloc(MEMORY_CAPACITY, sizeof(int8_t));
  
  if (curr_state->memory == NULL) {
    perror("coudn't initialize memory");	 
    exit(EXIT_FAILURE);
  }

}  

//free memory
void freeRegs(){
  for(int i = 0; i < GEN_PURPOSE_REG; i++){
    free(*(curr_state->registers + i));
  }

  free(curr_state->CPSR);
  free(curr_state->pipeline);
  free(curr_state->memory);
  free(curr_state);
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

  //checking arguments
  if (argc != 2) {
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }
 
  //initialize registers
  reg_init();

  //open file
  FILE *fptr = fopen(argv[1], "rb");
 
  //pass error if unvalid file
  if (!fptr) {
    printf("Unable to open file\n");
    return EXIT_FAILURE;
  }

  fseek(fptr, 0, SEEK_END);     //setting file point32_ter to last place
  int32_t file_size = ftell(fptr);  //ftell returns the pos of the file point32_ter
  rewind(fptr);           //rewind resets the file point32_ter to the start pos


  //int8_t littleEndianBuffer[file_size]; //store instruction
  fread(curr_state->memory, sizeof(int8_t), file_size, fptr);  

  //fetch the instruction and store in byte
  int8_t* byte; 
 
  while (1) {
    byte = fetchInstruction(curr_state->memory);
    curr_state->pipeline->fetched = instrToBits(byte);
//  for (int i = 31; i >= 0; i--){
//  printf("%d", *(curr_state->pipeline->fetched + i));}
//  printf("\n");
//  printf("While loop called.\n");
    if (allZeroes(curr_state->pipeline->fetched) == 1) {
      free(byte);
      curr_state->PC += 4;
      break;
    }
    if (byte != NULL) {
      free(byte);
    }                     //free instruction memory
    decode(curr_state->pipeline->fetched);
  }    

  //Outputting the state of Registers and non-zero memory
  
  printf("Registers:\n");

  for(int i = 0; i <= 12; i++) {
    int reg_value = convBinToDec(*(curr_state->registers + i), 32);
    printf("$%-3d: %10d (0x%08x)\n", i,reg_value,reg_value);
  }

  printf("PC  : %10d (0x%08x)\n", curr_state->PC, curr_state->PC);
  printf("CPSR: %10d (0x%08x)\n", convBinToDec(curr_state->CPSR, 32),convBinToDec(curr_state->CPSR, 32));


  printf("Non-zero memory:\n");

  for (int i = 0; i < MEMORY_CAPACITY; i+=4) {
    int8_t *membyte = curr_state->memory + i;
    if (!(*membyte == 0) || !(*(membyte+1) == 0) || !(*(membyte+2) == 0) || !(*(membyte+3) == 0)){
      printf("0x%08x: ", i);
      printf("0x%02hhx%02hhx%02hhx%02hhx\n",(*membyte),(*(membyte+1)),(*(membyte+2)),*(membyte+3));
    } 
  }
  fclose(fptr);                  //close file

  freeRegs();                    //free memory

  return EXIT_SUCCESS;
}

*/

