#ifndef _ARM_GEN
#define _ARM_GEN

// ------ ARM GENERAL DEFINITIONS -----------

// ------ CONSTANTS -------------------------

#define MEMORY_CAPACITY 65536
#define TOTAL_REGISTERS 17

// ------ PIPELINE --------------------------                                   

typedef struct Pipeline
{
    int32_t* fetched;
    int32_t* decoded;
} Pipeline;

// ------ CURRENT STATE  --------------------                                      

typedef struct CurrentState
{
    Pipeline *pipeline;
    int32_t registers[TOTAL_REGISTERS];
} CurrentState;

// ------ REGISTER DEFINE -------------------                              

typedef enum
{
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12,
    SP, LR,                  // ingored
    PC,                       
    CPSR
} Registers;

// ------ CPRS FLAG DEFINE ------------------                                   

typedef enum
{
    NEGATIVE = 31,
    ZERO     = 30,
    CARRY    = 29,
    OVERFLOW = 28
} CPSRFlags;


#endif
