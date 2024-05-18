/*Listen my brother
  this function is written by Mohammad Reihani 14030229
  and I riped my Fing a** to write this, so use it and remember me.

*/

// QuineMcCluskey.h

#ifndef QuineMcCluskey_h
#define QuineMcCluskey_h

// #include <Arduino.h>
// #include <avr/pgmspace.h>
#include <vector>
#include <utility>
#include <algorithm>


//this structure stores and element of QMC table!!(chi begam akhe!)
struct GroupedData {
  std::vector<int> mintermsIncluded;
  std::vector<int> deletedArgs;
  int stage;         //what stage of simlification is it in? (marhale chandom e sade sazie?)
  int groupFromTop;  //yeah, for example in initial shit, it starts from 0, 1 (for 1, 2, 4, 8), and so on
  bool isPI;
};

class QuineMcCluskey {
private:
  int* minterms;
  int numMinterms;
  int indicator;
  int numInputs;


  // std::vector<GroupedData> groupedTerms;
  // void addGroup(const std::vector<int>& mintermsIncluded, const std::vector<int>& deletedArgs, int stage, int groupFromTop);


  void initialMintermsGrouping();
  void groupMinterms();
  void markPrimeImplicants();
  void findEssentialPrimeImplicants();
  void simplifyBooleanExpression();
  void printSimplifiedExpression();
  int detectBitsCount();

  int countOnesInBinary(int num);
  bool isPowerOfTwo(int num);
  bool compareVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, bool considerOrder);
  void vectorSelectionSort(std::vector<int>& vec);
  bool vectorExists(const std::vector<int>& subVec, const std::vector<int>& mainVec);

public:
  QuineMcCluskey();
  QuineMcCluskey(int* inputArray, int length, int indicator);


  std::vector<GroupedData> groupedTerms;


  void solve();
};

#endif
