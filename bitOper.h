#include<stdint.h>
#include<stdio.h>

int getBit(int* num, int index);
void setBit(int* num, int givenBit, int index);
int getBits(int* num, int index, int size);
void setBits(int* target, int index_target, int* source, int index_source, int size);
