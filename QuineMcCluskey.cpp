// QuineMcCluskey.cpp


#include "QuineMcCluskey.h"


// Constructors :
QuineMcCluskey::QuineMcCluskey()= default;

QuineMcCluskey::QuineMcCluskey(int* mainTermsInputArray, int mainTermsInputArrayLength, bool maxTermsAreInput) {
    this->numMainTerms = mainTermsInputArrayLength;
    this->numAll = mainTermsInputArrayLength;
    this->allTerms = mainTermsInputArray;

    //Sort and delete all duplicates (allTerms)
    removeDuplicatesAndSortArray(allTerms, numAll);

    buildMainTerms(mainTermsInputArray);
    this->bitsNum = detectBitsCount();
    this->maxTermIsInput = maxTermsAreInput;
}

QuineMcCluskey::QuineMcCluskey(int* mainTermsInputArray, int mainTermsInputArrayLength, int* dontCaresInputArray, int dontCaresInputArrayLength, bool maxTermsAreInput) {
    this->numMainTerms = mainTermsInputArrayLength;
//    this->numDontCares = dontCaresInputArrayLength;
    this->numAll = mainTermsInputArrayLength + dontCaresInputArrayLength;

//    std::cout << "before the shit" << std::endl;

//    if (numAll > 0) {
//        std::memcpy(allTerms, mainTermsInputArray, mainTermsInputArrayLength * sizeof(int));
//        std::memcpy(allTerms + mainTermsInputArrayLength, dontCaresInputArray, dontCaresInputArrayLength * sizeof(int));
//    }

    allTerms = new int[numAll];
    std::copy(mainTermsInputArray, mainTermsInputArray + mainTermsInputArrayLength, allTerms);
    std::copy(dontCaresInputArray, dontCaresInputArray + dontCaresInputArrayLength,
              allTerms + mainTermsInputArrayLength);


    //Sort and delete all duplicates (allTerms)
    removeDuplicatesAndSortArray(allTerms, numAll);

//    for (int i = 0; i < numAll; ++i) {
//        std::cout << allTerms[i] << " ";
//    }
//    std::cout << std::endl;
//
//    std::cout << "after the shit" << std::endl;

    buildMainTerms(mainTermsInputArray);

    this->bitsNum = detectBitsCount();


    this->maxTermIsInput = maxTermsAreInput;
}


// Super Main Functions (there is just one function like this but, anyway):
// This function needs to be called before any action. ok?
// TODO Add a flag to check if the shit has been solved or not!
void QuineMcCluskey::solve() {
    // Make sure that you solve it only once!
    if(!isSolved) {
        initialMintermsGrouping();
        groupMinterms();
        markWasteImplicants();
        markPrimeImplicants();
        extractPrimeImplicants();
        markEssentialPrimeImplicants();
        updatedCoveredMainTerms();
        ApplyPetricksMethod();
        updatedCoveredMainTerms();//this is somehow redundant but let it be, we may be needing to print it!
    }

    isSolved = true; //marking as solved, so the user can move on and use other methods too! (like optional functions)
}

// Main Functions :
// these functions are the main steps taken to solve the problem

void QuineMcCluskey::initialMintermsGrouping() {
    int groupNum = 0;
    int totalAdded = 0;
    while (totalAdded < numAll) {
        for (int i = 0; i < numAll; i++) {
            if (countOnesInBinary(allTerms[i]) == groupNum) {
                groupedTerms.push_back({{allTerms[i] }, { /*none*/ }, 0, groupNum, false, false });
                totalAdded++;
            }
        }
        groupNum++;
    }
}

void QuineMcCluskey::groupMinterms() {
    int stage = 1;
    int startIndex;
    int endIndexHolder = 0;
    std::vector<int> combinedMinterms;
    std::vector<int> deletedArgs;
    while (true) {

        int groupFromTop = -1;
        int groupObserver = -1;

        startIndex = endIndexHolder;
        endIndexHolder = int(groupedTerms.size());
        bool anyAdded = false;
        for (int i = startIndex; i < endIndexHolder; i++) {
            const auto upperdata = groupedTerms[i];
            if (groupObserver != upperdata.groupFromTop) {
                groupFromTop++;
                groupObserver = upperdata.groupFromTop;
            }
            for (int j = i + 1; j < endIndexHolder; j++) {
                const auto& lowerdata = groupedTerms[j];

                if (upperdata.stage == lowerdata.stage && upperdata.groupFromTop + 1 == lowerdata.groupFromTop && isPowerOfTwo(lowerdata.termsIncluded[0] - upperdata.termsIncluded[0]) && compareVectors(upperdata.deletedArgs, lowerdata.deletedArgs, true)) {

                    combinedMinterms.clear();
                    combinedMinterms.insert(combinedMinterms.end(), upperdata.termsIncluded.begin(), upperdata.termsIncluded.end());
                    combinedMinterms.insert(combinedMinterms.end(), lowerdata.termsIncluded.begin(), lowerdata.termsIncluded.end());

                    deletedArgs.clear();
                    deletedArgs.insert(deletedArgs.end(), upperdata.deletedArgs.begin(), upperdata.deletedArgs.end());
                    deletedArgs.push_back(lowerdata.termsIncluded[0] - upperdata.termsIncluded[0]);

                    groupedTerms.push_back({ combinedMinterms, deletedArgs, stage, groupFromTop, false, false });
                    anyAdded = true;
                }
            }
        }


        if (!anyAdded)
            break;

        stage++;
    }
}

void QuineMcCluskey::markWasteImplicants() {
    int endIndexHolder = int(groupedTerms.size());
    for (int i = 0; i < endIndexHolder; i++) {

        auto& upperdata = groupedTerms[i];
        bool isWaste = false;

        for (int j = i + 1; j < endIndexHolder; j++) {
            const auto& lowerdata = groupedTerms[j];

            if (lowerdata.stage == upperdata.stage && lowerdata.groupFromTop == upperdata.groupFromTop && compareVectors(upperdata.termsIncluded, lowerdata.termsIncluded, false) && compareVectors(upperdata.deletedArgs, lowerdata.deletedArgs, false)) {
                isWaste = true;
                break;
            }


            if (lowerdata.stage != upperdata.stage)
                break;
        }

        upperdata.isWaste = isWaste;
    }
}

void QuineMcCluskey::markPrimeImplicants() {
    int endIndexHolder = int(groupedTerms.size());
    for (int i = 0; i < endIndexHolder; i++) {

        auto& upperdata = groupedTerms[i];
        bool existsInUpperStage = false;

        for (int j = i + 1; j < endIndexHolder; j++) {
            const auto& lowerdata = groupedTerms[j];

            if (lowerdata.stage == upperdata.stage + 1 && isSubVector(upperdata.termsIncluded, lowerdata.termsIncluded)) {
                existsInUpperStage = true;
                break;
            }


            if (lowerdata.stage > upperdata.stage + 1)
                break;
        }

        upperdata.isPI = !existsInUpperStage;

    }
}

void QuineMcCluskey::extractPrimeImplicants() {

    int endIndexHolder = int(groupedTerms.size());
    for (int i = 0; i < endIndexHolder; i++) {
        auto& data = groupedTerms[i];
        if(data.isPI && !data.isWaste){
            primeImplicants.push_back({data.termsIncluded, data.deletedArgs, -1, -1});
        }
    }


    // Release some memory : ALERT : ONLY UNCOMMENT THIS LINE IF MEMORY IS NOT AVAILABLE, OR IS LOW
//    groupedTerms.clear();
//    groupedTerms.shrink_to_fit();
}

void QuineMcCluskey::markEssentialPrimeImplicants() {
    int endIndexHolder = int(primeImplicants.size());
    for (int i = 0; i < endIndexHolder; i++) {
        auto& upperdata = primeImplicants[i];

        bool isEPI = false;
        for (int elem : upperdata.termsIncluded) {
            if (isMainTerm(elem)){
                bool found = false;
                for (int j = 0; j < endIndexHolder; j++) {
                    if(i == j)
                        continue;
                    const auto& lowerdata = primeImplicants[j];
                    for (int element : lowerdata.termsIncluded) {
                        if (element == elem) {
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                }
                if (!found) {
                    isEPI = true;
                    break;
                }
            }
        }

        if(upperdata.isEssential == -1)
            upperdata.isEssential = isEPI;
        if (isEPI && upperdata.isRequired == -1)
            upperdata.isRequired = isEPI;
    }

}

void QuineMcCluskey::ApplyPetricksMethod() {
    // going for Petricks method if needed. checking if there is any uncovered MainTerm, this should suffice right?
    if(uncoveredMainTermsRemained()){
        // I use index if unessential PI as it's label
        // and I have a vector that holds boolean expressions and then define methods to do that bool Algebra


        //looping over remaining mainTerms to create bool Expressions (I mean fill up the below vector):
        std::vector<std::vector<std::string>> Expressions;
        for (const auto & elem: MainTerms)
            if (!elem.isCovered){
                //loop over primeImplicants and push back the indexes into the shit
                int hold = int(primeImplicants.size());
                std::vector<std::string> temp;
                for (int i = 0; i < hold; ++i)
                    if(!primeImplicants[i].isEssential)
                        for (const auto & minterm : primeImplicants[i].termsIncluded)
                            if (elem.term == minterm){
                                temp.push_back(std::to_string(i));
                                break;
                            }
                Expressions.push_back(temp);


//                std::cout << "or elem : [ ";
//                for (std::string t: temp) {
//                    std::cout << t << " ";
//                }
//                std::cout << "]" << std::endl;
            }


        //by now, the initial P has been formed (Expression). now just we need to simplify it and convert it into SOP form
        //The expression is now in this state : ()()()
        //Now going for the simplification of Expressions :
        //We multiply the one in index i with i+1, if there is any, starting from i = 0; and in each
        //  turn that we perform multiplication, we perform simplification too :


        while (Expressions.size() > 1) {
            //multiply 0 with 1 and put in OrExpressions. then
            std::vector<std::string> tempResult;
            for (const auto& up: Expressions[0]) {
                for (const auto& down: Expressions[1]) {
                    tempResult.push_back(up + down);
                }
            }


            //Simplify tempResult :
            //  1. change these : 00 to 0 or 11 to 1 or 001 to 01 :
            for (auto & target : tempResult) {
                std::sort(target.begin(), target.end());
                target.erase(std::unique(target.begin(), target.end()), target.end());
            } // all of them will be sorted too! nice I guess


            //  2. change these : 0 + 01 to 0 | 1 + 123 to 1 :

            for (int i = 0; i < tempResult.size(); ++i) {
                for (int j = 0; j < tempResult.size(); ++j) {
                    if (i != j && isSubstringCustom(tempResult[j], tempResult[i])){
                        tempResult.erase(tempResult.begin() + j);  // Remove tempResult[j]
                        --j;
                        if (i > j)
                            --i;
                    }
                }
            }



            //delete 0 and 1 and push the tempResult at the beginning (consider size changing)
            if (Expressions.size() >= 2) {
                Expressions[0] = tempResult;  // Copy tempResult into Expressions[0]
                Expressions.erase(Expressions.begin() + 1);  // Erase Expressions[1]
            }

//            std::cout << "( ";
//            for (const auto & up : tempResult) {
//                std::cout << up << " ";
//            }
//            std::cout << ")(";
//            for (const auto & up : Expressions[1]) {
//                std::cout << up << " ";
//            }
//            std::cout << ")" << std::endl;

        }


//        for (const auto & up : Expressions[0]) {
//            std::cout << up << " ";
//        }
//        std::cout << std::endl;

        //  In this state, Expressions[0] holds all possible combinations of choosable PI's, now, we'll just choose the first one,
        //      that hold less number of PI indexes. and then mark those PI indexes as required. done
        //  We might have multiple choices here, I'm just gonna choose the first one from the begin. ok?
        std::string shortestString = Expressions[0][0];

        // Iterate through the vector to find the shortest string
        for (const auto& str : Expressions[0]) {
            if (str.length() < shortestString.length()) {
                shortestString = str;
            }
        }


        //Now, we're going to mark the related PI'S as required :
        for (const char & c: shortestString) {
            int index = c - '0';
            primeImplicants[index].isRequired = true;
        }
        //Done, now we just need to go for displaying, or creating a single expression, like ABC + A`BC and so on
    }
    //else, there is no need to apply this method, you can go for display part, based on the required PI's
}

void QuineMcCluskey::updatedCoveredMainTerms(){
    for(auto& minterm : MainTerms){
        if(!minterm.isCovered){//should it be like this?
            bool isCovered = false;
            for (const auto& primeI : primeImplicants){
                if(primeI.isRequired == 1){
                    for (int elem : primeI.termsIncluded){
                        if(minterm.term == elem){
                            isCovered = true;
                            break;
                        }
                    }
                    if(isCovered)
                        break;
                }
            }
            minterm.isCovered = isCovered;
        }
    }
}


// Sub-functions : functions used mostly in the main functions that are NOT static

void QuineMcCluskey::buildMainTerms(int* inputArray){
    std::sort(inputArray, inputArray + numMainTerms);  // Sort the input array first
    for (int i = 0; i < numMainTerms; i++){
        MainTerms.push_back({inputArray[i], false});
    }
}

//bool QuineMcCluskey::uncoveredMainTermsRemained(){
//    for (const auto & elem: MainTerms)
//        if (!elem.isCovered)
//            return true;
//    return false;
//}
//
//bool QuineMcCluskey::isMainTerm(const int& target){
//    for (const auto& elem : MainTerms)
//        if (target == elem.term)
//            return true;
//    return false;
//}

bool QuineMcCluskey::uncoveredMainTermsRemained() {
    return std::any_of(MainTerms.begin(), MainTerms.end(), [](const auto& elem) {
        return !elem.isCovered;
    });
}

bool QuineMcCluskey::isMainTerm(const int& target) {
    return std::find_if(MainTerms.begin(), MainTerms.end(), [&](const auto& elem) {
        return target == elem.term;
    }) != MainTerms.end();
}

int QuineMcCluskey::detectBitsCount() {
    int num = allTerms[numAll - 1];

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


// Optional Functions

void QuineMcCluskey::printSimplifiedExpression() {
    // Implement printing the simplified Boolean expression
}
// Optional : Getters
const std::vector<MainTerm> &QuineMcCluskey::getMainTerms() const {
    if (isSolved) {
        return MainTerms;
    } else {
        static const std::vector<MainTerm> emptyVector;  // Empty vector as placeholder
        return emptyVector;
    }
}

const std::vector<GroupedData> &QuineMcCluskey::getGroupedTerms() const {
    if (isSolved) {
        return groupedTerms;
    } else {
        static const std::vector<GroupedData> emptyVector;  // Empty vector as placeholder
        return emptyVector;
    }
}

const std::vector<PrimeImplicant> &QuineMcCluskey::getPrimeImplicants() const {
    if (isSolved) {
        return primeImplicants;
    } else {
        static const std::vector<PrimeImplicant> emptyVector;  // Empty vector as placeholder
        return emptyVector;
    }
}



// Static Functions (they are static but private! :) ):

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

void QuineMcCluskey::vectorSelectionSort(std::vector<int>& vec) {
    int n = int(vec.size());
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

bool QuineMcCluskey::isSubVector(const std::vector<int>& subVec, const std::vector<int>& mainVec) {

    if (subVec.size() > mainVec.size()) {
        return false;
    }

    for (int i = 0; i <= mainVec.size() - subVec.size(); i++) {
        if (std::equal(subVec.begin(), subVec.end(), mainVec.begin() + i)) {
            return true;
        }
    }

    return false;
}

void QuineMcCluskey::removeDuplicatesAndSortArray(int* arr, int& size) {
    std::sort(arr, arr + size); // Sort the array

    auto it = std::unique(arr, arr + size); // Remove adjacent duplicates
    size = int(std::distance(arr, it)); // Update the size of the array
}

//bool QuineMcCluskey::isSubstringCustom(const std::string& str1, const std::string& str2) {
//    for (char target : str2) {
//        if (str1.find(target) == std::string::npos) {
//            return false;  // If any character in str2 is not found in str1, return false
//        }
//    }
//    return true;  // All characters in str2 are found in str1
//}

bool QuineMcCluskey::isSubstringCustom(const std::string& str1, const std::string& str2) {
    return std::all_of(str2.begin(), str2.end(), [&](char target) {
        return str1.find(target) != std::string::npos;
    });
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

std::string QuineMcCluskey::getStringExpression() {
    if(isSolved){
        std::string result;
        for (const auto& elem: primeImplicants) {
            if (elem.isRequired == 1){
                result += maxTermIsInput ? "(" : "";
                result += termToExpression(elem.termsIncluded, elem.deletedArgs); //Check isMaxTermInput and bits count.
                result += maxTermIsInput ? ")" : "+";
            }
        }
        if (!maxTermIsInput)
            result.erase(result.size() - 1);
        return result;
    } else {
        return "not solved yet!";
    }
}

std::string QuineMcCluskey::termToExpression(std::vector<int> terms, std::vector<int> deletedArgs) {
    std::string Inputs[] = {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
            "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    };

    //turning deletedArgs to indexes :
    for (auto& arg: deletedArgs) {
        arg = bitsNum - 1 - int(log2(arg));
    }

    std::string result = intToBinaryString(terms[0], bitsNum);

    std::sort(deletedArgs.rbegin(), deletedArgs.rend());
    std::string abcRepresentation{};
    int counter = 0;
    for (int i = 0; i < bitsNum; ++i) {
        if (std::find(deletedArgs.begin(), deletedArgs.end(), i) != deletedArgs.end()) {
            // Skip deleted indices
            continue;
        }

        if(!maxTermIsInput){
            if (result[i] == '1') {
                abcRepresentation += Inputs[i];
            } else {
                abcRepresentation += Inputs[i] + "\'";
            }
        } else {
            if (result[i] == '1') {
                abcRepresentation += Inputs[i] + "\'";
            } else {
                abcRepresentation += Inputs[i];
            }
            if (counter != bitsNum-deletedArgs.size()-1){
                abcRepresentation += "+";
            }
        }
        counter++;
    }
    return abcRepresentation;
}

std::string QuineMcCluskey::intToBinaryString(const int& num, const int& bits) {

    std::bitset<128> binary(num);
    std::string binaryStr = binary.to_string();

    // Trim the string to the desired number of bits
    if (bits < 128) {
        binaryStr = binaryStr.substr(128 - bits);
    }

    return binaryStr;
}

