// QuineMcCluskey.cpp

#include "QuineMcCluskey.h"

QuineMcCluskey::QuineMcCluskey(){};

QuineMcCluskey::QuineMcCluskey(int* inputArray, int length, int indicator) {
  this->minterms = inputArray;
  this->numMinterms = length;
  this->indicator = indicator;
  this->numInputs = detectBitsCount();
}



void QuineMcCluskey::solve() {
  groupMinterms();
  generatePrimeImplicants();
  findEssentialPrimeImplicants();
  simplifyBooleanExpression();
  printSimplifiedExpression();
}


void QuineMcCluskey::groupMinterms() {
  // Implement grouping logic
}

void QuineMcCluskey::generatePrimeImplicants() {
  // Implement prime implicants generation logic
}

void QuineMcCluskey::findEssentialPrimeImplicants() {
  // Implement essential prime implicants logic
}

void QuineMcCluskey::simplifyBooleanExpression() {
  // Implement Boolean expression simplification logic
}

void QuineMcCluskey::printSimplifiedExpression() {
  // Implement printing the simplified Boolean expression
}



int QuineMcCluskey::detectBitsCount() {
  int num = minterms[numMinterms - 1];

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

// QuineMcCluskey::~QuineMcCluskey() {
//   delete[] inputArray;
// }
// void QuineMcCluskey::setup() {
//   // Example method implementation
//   // Perform setup tasks
// }
