#ifndef _ARM_GEN
#define _ARM_GEN

// ------ ARM GENERAL DEFINITIONS -----------

/*

Contains general definitions and structure for 
given Rasberry Pi system.  

*/


// ------ CONSTANTS -------------------------

// memory = 65536 bytes/ 4 words = 
#define MEMORY_CAPACITY (65536/4)
#define TOTAL_REGISTERS 17
#define GEN_PURPOSE_REG 13
#define INSTRUCTION_BYTE_SIZE 4

// ------ PIPELINE --------------------------                                   

typedef struct Pipeline
{
    int32_t fetched;
    int32_t decoded;
} Pipeline;

// ------ CURRENT STATE  --------------------                                      

typedef struct CurrentState
{
    Pipeline pipeline;
    int32_t registers[TOTAL_REGISTERS];
    int32_t memory[MEMORY_CAPACITY];
} CurrentState;


//------- INSTRUCTION TYPE ------------------

enum Instruction_type
{
    Data_processing,
    Multiply,
    Single_data_transfer,
    Branch 
};

#endif
