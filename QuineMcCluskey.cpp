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

  markPrimeImplicants();

  findEssentialPrimeImplicants();
  simplifyBooleanExpression();
  printSimplifiedExpression();
}

void QuineMcCluskey::groupMinterms() {
  int stage = 1;
  // int groupFromTop = 0;
  int startIndex;
  int endIndexHolder = 0;
  std::vector<int> combinedMinterms;
  std::vector<int> deletedArgs;
  while (true) {

    int groupFromTop = -1;
    int groupObserver = -1;

    startIndex = endIndexHolder;
    endIndexHolder = groupedTerms.size();
    bool anyAdded = false;
    for (int i = startIndex; i < endIndexHolder; i++) {
      const auto upperdata = groupedTerms[i];

      // if(groupObserver == -1)
      //   groupObserver = upperdata.groupFromTop;
      if (groupObserver != upperdata.groupFromTop) {
        groupFromTop++;
        groupObserver = upperdata.groupFromTop;
      }


      for (int j = i + 1; j < endIndexHolder; j++) {
        const auto& lowerdata = groupedTerms[j];

        if (upperdata.stage == lowerdata.stage && upperdata.groupFromTop + 1 == lowerdata.groupFromTop && isPowerOfTwo(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]) && compareVectors(upperdata.deletedArgs, lowerdata.deletedArgs, true)) {



          combinedMinterms.clear();
          combinedMinterms.insert(combinedMinterms.end(), upperdata.mintermsIncluded.begin(), upperdata.mintermsIncluded.end());
          combinedMinterms.insert(combinedMinterms.end(), lowerdata.mintermsIncluded.begin(), lowerdata.mintermsIncluded.end());

          deletedArgs.clear();
          deletedArgs.insert(deletedArgs.end(), upperdata.deletedArgs.begin(), upperdata.deletedArgs.end());
          //                    deletedArgs.insert(deletedArgs.end(), lowerdata.deletedArgs.begin(), lowerdata.deletedArgs.end());
          deletedArgs.push_back(lowerdata.mintermsIncluded[0] - upperdata.mintermsIncluded[0]);

          //                    addGroup(combinedMinterms, deletedArgs, stage, groupFromTop);
          groupedTerms.push_back({ combinedMinterms, deletedArgs, stage, groupFromTop, false });
          anyAdded = true;
        }
      }
      // groupFromTop++;
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
        groupedTerms.push_back({ { minterms[i] }, { /*none*/ }, 0, groupNum, false });
        totalAdded++;
      }
    }
    groupNum++;  // Move to the next group
  }
}

void QuineMcCluskey::markPrimeImplicants() {
  int endIndexHolder = groupedTerms.size();
  for (int i = 0; i < endIndexHolder; i++) {

    auto& upperdata = groupedTerms[i];
    bool existsInUpperStage = false;

    for (int j = i + 1; j < endIndexHolder; j++) {
      const auto& lowerdata = groupedTerms[j];

      if (lowerdata.stage == upperdata.stage + 1 && vectorExists(upperdata.mintermsIncluded, lowerdata.mintermsIncluded)) {
        existsInUpperStage = true;
        break;
      }


      if (lowerdata.stage > upperdata.stage + 1)
        break;
    }

    if (!existsInUpperStage) {
      upperdata.isPI = true;
    }
  }
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

bool QuineMcCluskey::compareVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, bool considerOrder) {
  if (vec1.size() != vec2.size()) {
    return false;  // Vectors of different sizes cannot be equal
  }

  if (considerOrder) {
    return vec1 == vec2;  // Compare vectors directly for equality with order consideration
  } else {
    // Sort the vectors and then compare for equality
    std::vector<int> sortedVec1 = vec1;
    std::vector<int> sortedVec2 = vec2;
    vectorSelectionSort(sortedVec1);
    vectorSelectionSort(sortedVec2);
    return sortedVec1 == sortedVec2;
  }
}

void QuineMcCluskey::vectorSelectionSort(std::vector<int>& vec) {
  int n = vec.size();
  for (int i = 0; i < n - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < n; j++) {
      if (vec[j] < vec[minIndex]) {
        minIndex = j;
      }
    }
    // Swap vec[i] and vec[minIndex]
    int temp = vec[i];
    vec[i] = vec[minIndex];
    vec[minIndex] = temp;
  }
}



bool QuineMcCluskey::vectorExists(const std::vector<int>& subVec, const std::vector<int>& mainVec) {
  if (subVec.size() > mainVec.size()) {
    return false;  // Subvector cannot be larger than main vector
  }

  for (size_t i = 0; i <= mainVec.size() - subVec.size(); i++) {
    if (std::equal(subVec.begin(), subVec.end(), mainVec.begin() + i)) {
      return true;  // Subvector found in main vector
    }
  }

  return false;  // Subvector not found in main vector
}

// QuineMcCluskey::~QuineMcCluskey() {
//   delete[] inputArray;
// }
// void QuineMcCluskey::setup() {
//   // Example method implementation
//   // Perform setup tasks
// }
