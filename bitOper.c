#include"bitOper.h"
#include"ARMgen.h"

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

int32_t * rotate_right(int32_t * array, int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {
    int temp;
    temp = *(array + size - 1);
    for (int i = size - 1; i > 0; i--) {
      *(array + i) = *(array + i - 1);
    }
    *(array + 0) = temp;
  }
  return array;
}

int32_t * shift_left(int32_t * array, int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {

    updateCarry(*(array));

    for (int i = size - 1; i > 0; i--) {
      *(array + i) = *(array + i - 1);
    }
    *(array) = 0;
  }
  return array;
}

int32_t * shift_right(int32_t * array, int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {

    updateCarry(*(array));

    for (int i = size - 2; i >= 0; i--) {
      *(array + i) = *(array + i + 1);
    }
    *(array + size - 1) = 0;
  }
  return array;
}

int32_t * arith_shift_right(int32_t * array, int size, int rot_num) {
  for (int n = 0; n < rot_num; n++) {
    updateCarry(*(array));

    for (int i = size - 3; i >= 0; i--) {
      *(array + i) = *(array + i + 1);
    }
    *(array + size - 2) = 0;
  }
  return array;
}


int32_t * binary_sub(int32_t * arr1, int32_t * arr2, int size) {
  int carry = 0;
  static int32_t * resArray;
    for (int i = 0; i < size; i++) {
      *(resArray + i) = *(arr1 + i) - *(arr2 + i) - carry;
      if (*(resArray + i) < 0) {
        *(resArray) += 2;
        carry = 1;
      } else{
        carry = 0;
      }
    }
  updateCarry(carry);
  return resArray;
}

int32_t * binary_add(int32_t * arr1, int32_t * arr2, int size) {
  int carry = 0;
  static int32_t * resArray;
    for (int i = 0; i < size; i++) {
      *(resArray + i) = *(arr1 + i) + *(arr2 + i) + carry;
      if (*(resArray + i) > 1) {
        *(resArray) -= 2;
        carry = 1;
      } else{
        carry = 0;
      }
    }
  updateCarry(carry);
  return resArray;
}






