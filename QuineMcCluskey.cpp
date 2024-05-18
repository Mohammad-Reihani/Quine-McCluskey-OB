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
  initialMintermsGrouping();
  groupMinterms();
  generatePrimeImplicants();
  findEssentialPrimeImplicants();
  simplifyBooleanExpression();
  printSimplifiedExpression();
}

void QuineMcCluskey::addGroup(const std::vector<int>& mintermsIncluded, const std::vector<int>& deletedArgs, int stage, int groupFromTop) {
    groupedTerms.push_back({mintermsIncluded, deletedArgs, stage, groupFromTop});
}

void QuineMcCluskey::groupMinterms() {

}

void QuineMcCluskey::initialMintermsGrouping() {
  int groupNum = 0;
  int totalAdded = 0;

  // Loop until all minterms are grouped
  while (totalAdded < numMinterms) {
    for (int i = 0; i < numMinterms; i++) {
      if (countOnesInBinary(minterms[i]) == groupNum) {
        // Push a pair into groupedTerms
        // groupedTerms.push_back(std::make_pair(minterms[i], groupNum));
        addGroup({minterms[i]}, {/*none*/}, 0, groupNum);
        totalAdded++;
      }
    }
    groupNum++;  // Move to the next group
  }
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

int QuineMcCluskey::countOnesInBinary(int num) {
  int count = 0;

  // Convert num to binary and count 1's
  while (num > 0) {
    if (num % 2 == 1) {
      count++;
    }
    num /= 2;
  }

  return count;
}

// QuineMcCluskey::~QuineMcCluskey() {
//   delete[] inputArray;
// }
// void QuineMcCluskey::setup() {
//   // Example method implementation
//   // Perform setup tasks
// }
