#ifndef _ARM_GEN
#define _ARM_GEN

// ------ ARM GENERAL DEFINITIONS -----------

// ------ CONSTANTS -------------------------

#define MEMORY_CAPACITY 65536
#define TOTAL_REGISTERS 17
#define GEN_PURPOSE_REG 13
#define INSTRUCTION_BYTE_SIZE 4


//------- DECODED INSTRUCTION ---------------
typedef struct Decoded
{
  int8_t condition;
  int instruction_type; // 1=data processing, 2=multyply, 3=sdt, 4=branch
  int8_t opcode;        // for DP
  int accSwitch;        // for multiply
  int sSwitch;          // should the flags be set
  int8_t rn;
  int8_t rd;
  int8_t rs;
  int8_t rm;
  int32_t endVal;      // operand2 in case of DP, offset in sdt and branch

} Decoded; 


// ------PIPELINE --------------------------                                   

typedef struct Pipeline
{
    int32_t* fetched;
    Decoded decoded;
} Pipeline;

// ------ CURRENT STATE  --------------------                                      

typedef struct CurrentState
{
    Pipeline *pipeline;
    int32_t* registers[GEN_PURPOSE_REG];
    int PC;
    int32_t* CPSR;
    int8_t* memory;
} CurrentState;

void updateCarry(int carry);
int * binary_sub(int32_t * arr1, int32_t * arr2, int size);
int * binary_add(int32_t * arr1, int32_t * arr2, int size); 
#endif
