#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <cmath>
#include <bitset>

//this structure stores element of QMC table!!(chi begam akhe!) | so many things are in here, just take a look
struct GroupedData {
    std::vector<int> termsIncluded;
    std::vector<int> deletedArgs;
    int stage;         //what stage of simplification is it in? actually indicates the column of the table
    int groupFromTop;  //yeah, for example in initial shit, it starts from 0, 1 (for 1, 2, 4, 8), and so on
    bool isPI;
    bool isWaste;
};

struct PrimeImplicant {
    std::vector<int> termsIncluded;
    std::vector<int> deletedArgs;
    int isEssential;
    int isRequired;
};

struct MainTerm {
    int term;
    bool isCovered;
};

class QuineMcCluskey {
private:
    // Some very important and private variables :
    bool isSolved = false;
    int* allTerms{}; //includes dont-care's too
    int numMainTerms{};
//    int numDontCares;
    int numAll{};
    int maxTermIsInput{};
    int bitsNum{};

    std::vector<MainTerm> MainTerms; // Just mainTerms (minTerms or maxTerms are stored here)
    std::vector<GroupedData> groupedTerms; //Both mainTerms and don't cares are going to be included here
    std::vector<PrimeImplicant> primeImplicants; //Both mainTerms and don't cares are going to be included here


    // Main functions :
    void initialMintermsGrouping();
    void groupMinterms();
    void markWasteImplicants();
    void markPrimeImplicants();
    void extractPrimeImplicants(); //extracts the calculated PI's and then deletes the heavy vector to free up memory
    void markEssentialPrimeImplicants();
    void updatedCoveredMainTerms();
    void ApplyPetricksMethod();



    // Sub-functions :
    int detectBitsCount();
    void buildMainTerms(int* inputArray);
    bool uncoveredMainTermsRemained();
    bool isMainTerm(const int& target);
    std::string termToExpression(std::vector<int> terms, std::vector<int> deletedArgs);



    // Static Functions :
    static int countOnesInBinary(int num);
    static bool isPowerOfTwo(int num);
    static bool compareVectors(const std::vector<int>& vec1, const std::vector<int>& vec2, bool considerOrder);
    static void vectorSelectionSort(std::vector<int>& vec);
    static bool isSubVector(const std::vector<int>& subVec, const std::vector<int>& mainVec);
    static bool isSubstringCustom(const std::string& str1, const std::string& str2);
    static void removeDuplicatesAndSortArray(int* arr, int& size);
    static std::string intToBinaryString(const int& num , const int& bits);
public:
    // Constructors :
    QuineMcCluskey();
    QuineMcCluskey(int* mainTermsInputArray, int mainTermsInputArrayLength, bool maxTermsAreInput);//The constructor for NO DON'T CARE'S
    QuineMcCluskey(int* mainTermsInputArray, int mainTermsInputArrayLength, int* dontCaresInputArray, int dontCaresInputArrayLength, bool maxTermsAreInput);//The constructor for NO DON'T CARE'S

    // Optional Functions :
    void printSimplifiedExpression();
    std::string getStringExpression();

    // Optional : Getters :
    const std::vector<MainTerm> &getMainTerms() const;
    const std::vector<GroupedData> &getGroupedTerms() const;
    const std::vector<PrimeImplicant> &getPrimeImplicants() const;
    void solve();
};