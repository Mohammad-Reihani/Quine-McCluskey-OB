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

// void QuineMcCluskey::addGroup(const std::vector<int>& mintermsIncluded, const std::vector<int>& deletedArgs, int stage, int groupFromTop) {
//   groupedTerms.push_back({ mintermsIncluded, deletedArgs, stage, groupFromTop });
// }

// void QuineMcCluskey::groupMinterms() {
//   int stage = 1;
//   int groupFromTop = 0;
//   int added = 0;
//   while (true) {
//     // for (const auto& upperdata : testQMC.groupedTerms) {
//     //   for (const auto& lowerdata : testQMC.groupedTerms) {
//     //     if(upperdata != lowerdata && upperdata.stage == lowerdata.stage && upperdata.group+1 == lowerdata.group
//     //      && isPowerOfTwo(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0])){
//     //         //do something
//     //     }
//     //   }
//     // }
//     for (size_t i = 0; i < groupedTerms.size(); i++) {
//       const auto& upperdata = groupedTerms[i];
//       for (size_t j = i + 1; j < groupedTerms.size(); j++) {
//         const auto& lowerdata = groupedTerms[j];
//         if (upperdata.stage == lowerdata.stage && upperdata.groupFromTop + 1 == lowerdata.groupFromTop && isPowerOfTwo(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0])) {
//           // addGroup(upperdata.mintermsIncluded.push_back(lowerdata.mintermsIncluded), lowerdata.deletedArgs.push_back(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]), stage, groupNum);

//           // Combine mintermsIncluded vectors of upperdata and lowerdata
//           std::vector<int> combinedMinterms;
//           combinedMinterms.insert(combinedMinterms.end(), upperdata.mintermsIncluded.begin(), upperdata.mintermsIncluded.end());
//           combinedMinterms.insert(combinedMinterms.end(), lowerdata.mintermsIncluded.begin(), lowerdata.mintermsIncluded.end());

//           // Add the combined vector to deletedArgs
//           std::vector<int> deletedArgs;
//           deletedArgs.push_back(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]);

//           // Call addGroup with combined data
//           addGroup(combinedMinterms, deletedArgs, stage, groupFromTop);
//           added++;
//         }
//       }
//       groupFromTop++;
//     }

//     if (added == 0)
//       break;
//     else
//       added = 0;

//     stage++;
//   }
// }

// void QuineMcCluskey::groupMinterms() {
//   int stage = 1;
//   int groupFromTop = 0;
//   int added = 0;
//   std::vector<int> combinedMinterms;
//   std::vector<int> deletedArgs;

//   while (true) {
//     bool anyAdded = false;


//     for (int i = 0; i < groupedTerms.size(); i++) {
//       const auto upperdata = groupedTerms[i];
//       for (int j = i + 1; j < groupedTerms.size(); j++) {
//         const auto lowerdata = groupedTerms[j];
//         if (upperdata.stage == lowerdata.stage && upperdata.groupFromTop + 1 == lowerdata.groupFromTop && isPowerOfTwo(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0])) {
//           combinedMinterms.clear();
//           combinedMinterms.insert(combinedMinterms.end(), upperdata.mintermsIncluded.begin(), upperdata.mintermsIncluded.end());
//           combinedMinterms.insert(combinedMinterms.end(), lowerdata.mintermsIncluded.begin(), lowerdata.mintermsIncluded.end());

//           deletedArgs.clear();
//           deletedArgs.push_back(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]);

//           // addGroup(combinedMinterms, deletedArgs, stage, groupFromTop);
//           groupedTerms.push_back({ combinedMinterms, deletedArgs, stage, groupFromTop });
//           added++;
//           anyAdded = true;
//         }
//       }
//       groupFromTop++;
//     }

//     if (!anyAdded)
//       break;

//     stage++;
//   }
// }


void QuineMcCluskey::groupMinterms() {
  int stage = 1;
  int groupFromTop = 0;
  int startIndex;
  int endIndexHolder = 0;
  std::vector<int> combinedMinterms;
  std::vector<int> deletedArgs;
  while (true) {

    startIndex = endIndexHolder;
    endIndexHolder = groupedTerms.size();
    bool anyAdded = false;
    for (int i = startIndex; i < endIndexHolder; i++) {

      const auto upperdata = groupedTerms[i];
      for (int j = i + 1; j < endIndexHolder; j++) {
        const auto& lowerdata = groupedTerms[j];

        if (upperdata.stage == lowerdata.stage && upperdata.groupFromTop + 1 == lowerdata.groupFromTop && isPowerOfTwo(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0])) {



          combinedMinterms.clear();
          combinedMinterms.insert(combinedMinterms.end(), upperdata.mintermsIncluded.begin(), upperdata.mintermsIncluded.end());
          combinedMinterms.insert(combinedMinterms.end(), lowerdata.mintermsIncluded.begin(), lowerdata.mintermsIncluded.end());

          deletedArgs.clear();
          deletedArgs.insert(deletedArgs.end(), upperdata.deletedArgs.begin(), upperdata.deletedArgs.end());
          //                    deletedArgs.insert(deletedArgs.end(), lowerdata.deletedArgs.begin(), lowerdata.deletedArgs.end());
          deletedArgs.push_back(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]);

          //                    addGroup(combinedMinterms, deletedArgs, stage, groupFromTop);
          groupedTerms.push_back({ combinedMinterms, deletedArgs, stage, groupFromTop });
          anyAdded = true;
        }
      }
      groupFromTop++;
    }


    if (!anyAdded)
      break;

    stage++;
  }
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
        // addGroup({ minterms[i] }, { /*none*/ }, 0, groupNum);
        groupedTerms.push_back({ { minterms[i] }, { /*none*/ }, 0, groupNum });
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

bool QuineMcCluskey::isPowerOfTwo(int num) {
  if (num <= 0) {
    return false;  // Negative numbers and zero are not powers of two
  }

  // Check if num is a power of two by counting the set bits (1s) in its binary representation
  return (num & (num - 1)) == 0;
}

// QuineMcCluskey::~QuineMcCluskey() {
//   delete[] inputArray;
// }
// void QuineMcCluskey::setup() {
//   // Example method implementation
//   // Perform setup tasks
// }
