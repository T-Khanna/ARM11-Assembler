////////////////////////////////////////////////////////////////////////////////
// ARM Group Project - Year 1 (Group 40)
// _____________________________________________________________________________
// 
// File: execute_helper.c
// Members: Tarun Sabbineni, Vinamra Agrawal, Tanmay Khanna, Balint Babik
////////////////////////////////////////////////////////////////////////////////


//------------------------ EMULATE HELPER HEADER -------------------------------
// Contains the function definitions for emulate.c, the execute function in 
// particular to carry out the four instructions specified.



//-- FUNCTION DECLARATIONS -----------------------------------------------------

void branch(int32_t* instr, current_state* curr_state);
void dataProcessing(int32_t* instr, current_state* curr_state);
void multiply(int32_t* instr, current_state* curr_state);
void singleDataTransfer(int32_t* instr, current_state* curr_state);

