// QuineMcCluskey.cpp

#include "QuineMcCluskey.h"

QuineMcCluskey::QuineMcCluskey(){};

QuineMcCluskey::QuineMcCluskey(int* inputArray, int length, int indicator) {
  // Constructor implementation
  this->inputArray = inputArray;
  this->arrayLength = length;
  this->indicator = indicator;
}

// QuineMcCluskey::~QuineMcCluskey() {
//   // Destructor implementation
//   // Clean up any resources allocated by the object

//   // Example: Free dynamically allocated memory
//   // delete[] inputArray;
// }

void QuineMcCluskey::setup() {
  // Example method implementation
  // Perform setup tasks
}

void QuineMcCluskey::solve() {
  // Example method implementation
  // Perform solving tasks
}



int QuineMcCluskey::detectBitsCount() {
    int num = inputArray[arrayLength - 1];

    // If num is already a power of 2, return it
    if (num && !(num & (num - 1))) {
        int power = 0;
        while (num >>= 1) {
            power++;
        }
        return power;
    }

    // Find the next power of 2 using bit manipulation
    int power = 1;
    while ((1 << power) < num) {
        power++;
    }

    return power;
}

