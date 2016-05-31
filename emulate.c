////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
// 
// File: emulate.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//-------------------------------- EMULATE -------------------------------------
// Holds the program that simulates the execution of an ARM binary ﬁle 
// on a Raspberry Pi.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARMgen.h"
#include "bitOper.h"
#include "execute_helper.h"


//-- FUNCTION DECLARATIONS -----------------------------------------------------

void regInit();
void printOutput();
int convEndian(int32_t num);
int checkCondition(int32_t* instr);
int32_t decode(int32_t* instr);


//-- GLOBAL VARIABLES ----------------------------------------------------------

//curr_state holds the state of registers, memory and the pipeline
current_state curr_state;


//-- MAIN ----------------------------------------------------------------------

int32_t main(int argc, char *argv[]) {

  //checking for an invalid number of arguments
  if(argc != 2){
    printf("Expecting one argument\n");
    return EXIT_FAILURE;
  }

  //initialises the memory, pipeline and registers
  regInit(argv[1]);

  //fetch, decode, execute cycle
  do {

    //fetch instruction
    curr_state.pipeline.fetched = curr_state.memory[curr_state.PC];

    //increment the PC
    curr_state.PC++;

    //checking condition (COND) for decoding and executing
    int goAhead = checkCondition(&curr_state.pipeline.fetched);

    //decode
    if(goAhead){

      //decode current fetched instruction in pipeline and store as the 
      //decoded instruction in the pipeline.
      curr_state.pipeline.decoded = decode(&curr_state.pipeline.fetched);

      //execute appropriate instruction depending on decoded instruction
      switch(curr_state.pipeline.decoded){

        case Data_processing:
          dataProcessing(&curr_state.pipeline.fetched, &curr_state);
          break;
<<<<<<< HEAD

        case Multiply:
          multiply(&curr_state.pipeline.fetched,&curr_state);
=======
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
>>>>>>> balint
          break;

        case Single_data_transfer:
          singleDataTransfer(&curr_state.pipeline.fetched, &curr_state);
          break;

        case Branch:
          branch(&curr_state.pipeline.fetched,&curr_state);
          break;

      }

    }

  } while(curr_state.pipeline.fetched != 0); //fetch, decode, execute cycle

  //print output of state of registers and non-zero memory.
  printOutput();

  return EXIT_SUCCESS;
}


//-- SUPPORTING FUNCTIONS ------------------------------------------------------

//initialises registers and memory by setting them to 0.
void regInit(char* file) {

  //setting all registers to 0
  for(int i = 0; i < TOTAL_REGISTERS; i++){
    curr_state.registers[i] = 0;
  }

  //clearing the pipeline
  curr_state.pipeline.fetched = 0;
  curr_state.pipeline.decoded = 0;

  //setting all memory to 0
  for(int i = 0; i < MEMORY_CAPACITY; i++){
    curr_state.memory[i] = 0;
  } 

  //moving instructions into memory
  FILE *fptr = fopen(file,"rb");

  //passing an error if file is invalid
  if(fptr == NULL){
    printf("Unable to open file\n");
    return;
  }

  //setting file point32_ter to the end of the file
  fseek(fptr, 0, SEEK_END);         

  //ftell returns the pos of the file point32_ter
  int file_size = ftell(fptr);	
  rewind(fptr); 

  //coverting bytes to word   
  file_size /= 4;

  //saving the file data in memory
  for(int i = 0; i < file_size; i++){
    fread(&(curr_state.memory[i]), sizeof(int32_t), 1, fptr);
  }

  //closing the binary input file
  fclose(fptr);

}

<<<<<<< HEAD
=======

//NEW
void execute(Decoded dec) {

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
}


void decode(int32_t * inst) {
  // set flags
  int32_t cond = 0,  goahead = 0 ;
  for (int i = 31; i >= 28; i--) {
    cond = cond * 2 + inst[i];
  }
 
>>>>>>> balint

//converts instruction from one Endian to the other
int convEndian(int num) {

  int result = 0;

  //for each byte in the instruction, obtains bits and places them in the 
  //rightmost byte position. Then left shifts them the appropriate number 
  //of times and stores them in the result to change endianness
  for(int i = 0; i < INSTRUCTION_BYTE_SIZE; i++){
    result = result | (getBits(&num, i * BYTE_SIZE, BYTE_SIZE) //gets a byte
             << ((INSTRUCTION_BYTE_SIZE -1 - i) * BYTE_SIZE));
  }

  return result;

<<<<<<< HEAD
}
=======
  currState = calloc(1,sizeof(CurrentState));
  currState->pipeline = calloc(1,sizeof(Pipeline));
  //New
  currState->pipeline->decoded = calloc(1,sizeof(Decoded));
>>>>>>> balint


//printing state of registers and all non-zero memory
void printOutput() {

  //printing state of general purpose registers
  printf("Registers:\n");

  for(int i = 0; i < GEN_PURPOSE_REG; i++){

    int reg_value = curr_state.registers[i];
    printf("$%-3d: %10d (0x%08x)\n", i, reg_value, reg_value);

  }

  //printing state of PC and CPSR registers
  printf("PC  : %10d (0x%08x)\n", INSTRUCTION_BYTE_SIZE*curr_state.PC +4,
      INSTRUCTION_BYTE_SIZE*curr_state.PC +4);
  printf("CPSR: %10d (0x%08x)\n", curr_state.CPSR, curr_state.CPSR);

  //printing non-zero memory
  printf("Non-zero memory:\n");

  for(int i = 0; i < MEMORY_CAPACITY; i++){

    //converting instructions from little endian to big endian 
    int32_t memWord = convEndian(curr_state.memory[i]);

    if(!(memWord == 0)){  //checking for non-zero memory
      printf("0x%08x: ", INSTRUCTION_BYTE_SIZE*i);
      printf("0x%08x\n",  memWord);
    }       

  }

}


//checks condition (COND) to decode and execute the instructions 
//locations of flags: Nflag = 31, Zflag = 30, Cflag = 29, Vflag = 28
int checkCondition(int32_t* instr) {

  int case_num = getBits(instr, 28 , 4);

  switch(case_num){

    //check z flag is set
    case 0:  return getBit(&curr_state.CPSR,30);

             //check z flag is clear
    case 1:  if(getBit(&curr_state.CPSR,30) == 0){         
               return 1;
             } else {return 0;}

             //check n equal to v
    case 10: if(getBit(&curr_state.CPSR,31) == getBit(&curr_state.CPSR,28)){
               return 1;
             } else {return 0;} 

             //n not equal to v
    case 11: if(getBit(&curr_state.CPSR,31) != getBit(&curr_state.CPSR,28)){
               return 1;
             } else {return 0;}

             //check z clear and n equal to v
    case 12: if((getBit(&curr_state.CPSR,31) 
                   == getBit(&curr_state.CPSR,28))
                 && getBit(&curr_state.CPSR,30) == 0){
               return 1;
             } else {return 0;}

             //check z or (n not equal to v)
    case 13: if((getBit(&curr_state.CPSR,30)==1) 
                 || (getBit(&curr_state.CPSR,31)
                   != getBit(&curr_state.CPSR,28))){
               return 1;
             } else {return 0;}

             //all flags
    case 14: return 1;

    default: return 0;

  }
}

<<<<<<< HEAD

//decodes the instruction and returns it as a decoded integer, depending upon 
//the given conditions for the appropriate bits.
int32_t decode(int32_t* instr) {

  //fetching bits 26 and 27
  int bit26 = getBit(instr,26);
  int bit27 = getBit(instr,27);  

  //if bit 26 and 27 are 0 then the instruction can be Multiply or 
  //Data Processing
  if((bit26==0) && (bit27==0)){

    //if I = 1 then the instruction is Data Processing 
    if(getBit(instr,25)){
      return Data_processing;
    }
=======

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
  // NEW
  // initializing nescessary stuff for while loop
  Decoded dCode = NULL;
  int done = 0;
 
 
  //NEW
  // initialise pipeline
  currState->pipeline->fetched = NULL;
  currState->pipeline->decoded = NULL;

/*
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
*/


  //NEW
  //decode gives a struct that describes the decoded code 
  while(done == 0) {
    if (currState->pipeline->fetched == NULL) {
      //first step
      byte = fetchInstruction(currState->memory);
      currState->pipeline->fetched = instrToBits(byte);

    } else if (currState->pipeline->decoded == NULL) {
      // second step
      dCoded = decode(currState->pipeline->fetched); //we could use memcpy  
      currState->pipeline->decoded = dCoded;
      
      byte = fetchInstruction(currState->memory);
      currState->pipeline->fetched = instrToBits(byte);
    } else {
    
      execute(currState->pipeline->decoded);
      
      dCoded = decode(currState->pipeline->fetched); //we could use memcpy  
      currState->pipeline->decoded = dCoded;

      byte = fetchInstruction(currState->memory);
      currState->pipeline->fetched = instrToBits(byte);
    }

                            
    }
    if (byte != NULL) {
      free(byte);           
    }// free instruction memory
    if (dCoded !=NULL) {
      free(dCoded);
    }
  }  
    




  //Outputting the state of Registers and non-zero memory
  
  printf("Registers:\n");
>>>>>>> balint

    if(getBit(instr,4)){

      //if bit 4 and 7 are set then the instruction is Multiply
      if(getBit(instr,7)){
        return Multiply;
      }

    }

    //otherwise, it is Data Processing
    return Data_processing;

  }

<<<<<<< HEAD
  //if bit 26 is 1. then the instruction is Single Data Transfer 
  else if((bit26==1) && (bit27==0)){
    return Single_data_transfer;
  }

  //otherwise, if none of the above cases pass, the instruction is Branch
  else {
    return Branch;
  }

=======
  freeRegs();                    // free memory
 
  return EXIT_SUCCESS;
>>>>>>> balint
}

