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
    bool isWaste;
};

struct PrimeImplicant {
    std::vector<int> mintermsIncluded;
    std::vector<int> deletedArgs;
    int isEssential;
    int isRequired;
};

struct Minterm {
    int minterm;
    bool isCovered;
};

class QuineMcCluskey {
private:
//    int* minterms;
    std::vector<Minterm> Minterms;
    int numMinterms;
    int maxtermIsInput;
    int bitsNum;


    void initialMintermsGrouping();
    void groupMinterms();

    void markWasteImplicants();

    void markPrimeImplicants();

    void extractPrimeImplicants(); //extracts the calculated PI's and then deletes the heavy vector to free up memory



    void markEssentialPrimeImplicants();
    void simplifyBooleanExpression();
    void printSimplifiedExpression();
    int detectBitsCount();

    int countOnesInBinary(int num);
    bool isPowerOfTwo(int num);
    bool compareVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, bool considerOrder);
    void vectorSelectionSort(std::vector<int>& vec);
    bool vectorExists(const std::vector<int>& subVec, const std::vector<int>& mainVec);
    void buildMinterms(int* inputArray);
    bool hasCommonElement(const std::vector<int>& vec1, const std::vector<int>& vec2);
public:
    QuineMcCluskey();
    QuineMcCluskey(int* inputArray, int length, bool maxtermIsInput);


    std::vector<GroupedData> groupedTerms;
    std::vector<PrimeImplicant> primeImplicants;


    void solve();
};
#endif
