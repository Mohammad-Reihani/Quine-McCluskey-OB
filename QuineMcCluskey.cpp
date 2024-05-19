// QuineMcCluskey.cpp


#include "QuineMcCluskey.h"

QuineMcCluskey::QuineMcCluskey(){};

QuineMcCluskey::QuineMcCluskey(int* inputArray, int length, bool maxtermIsInput) {
//    this->minterms = inputArray;

    //The Order of these matters!
    this->numMinterms = length;
    this->maxtermIsInput = maxtermIsInput;
    buildMinterms(inputArray);
    this->bitsNum = detectBitsCount();
}



void QuineMcCluskey::solve() {
    initialMintermsGrouping();
    groupMinterms();
    markWasteImplicants();
    markPrimeImplicants();
    extractPrimeImplicants();
    markEssentialPrimeImplicants();

    updatedCoveredMinterms();


    ApplyPetricsMethod();


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
                    groupedTerms.push_back({ combinedMinterms, deletedArgs, stage, groupFromTop, false, false });
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
            if (countOnesInBinary(Minterms[i].minterm) == groupNum) {
                // Push a pair into groupedTerms
                // groupedTerms.push_back(std::make_pair(minterms[i], groupNum));
                // addGroup({ minterms[i] }, { /*none*/ }, 0, groupNum);
                groupedTerms.push_back({ { Minterms[i].minterm }, { /*none*/ }, 0, groupNum, false, false });
                totalAdded++;
            }
        }
        groupNum++;  // Move to the next group
    }
}

void QuineMcCluskey::markWasteImplicants() {
    int endIndexHolder = groupedTerms.size();
    for (int i = 0; i < endIndexHolder; i++) {

        auto& upperdata = groupedTerms[i];
        bool isWaste = false;

        for (int j = i + 1; j < endIndexHolder; j++) {
            const auto& lowerdata = groupedTerms[j];

            if (lowerdata.stage == upperdata.stage && lowerdata.groupFromTop == upperdata.groupFromTop && compareVectors(upperdata.mintermsIncluded, lowerdata.mintermsIncluded, false) && compareVectors(upperdata.deletedArgs, lowerdata.deletedArgs, false)) {
                isWaste = true;
                break;
            }


            if (lowerdata.stage != upperdata.stage)
                break;
        }


        upperdata.isWaste = isWaste;
    }
};

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

        // if (!existsInUpperStage) {
        upperdata.isPI = !existsInUpperStage;
        // }
    }
}

void QuineMcCluskey::extractPrimeImplicants() {

    int endIndexHolder = groupedTerms.size();
    for (int i = 0; i < endIndexHolder; i++) {
        auto& data = groupedTerms[i];
        if(data.isPI && !data.isWaste){
            primeImplicants.push_back({data.mintermsIncluded, data.deletedArgs, -1, -1});
        }
    }


    groupedTerms.clear();          // Remove all elements from the vector
    groupedTerms.shrink_to_fit();  // Release excess memory
}

void QuineMcCluskey::markEssentialPrimeImplicants() {
//    int endIndexHolder = primeImplicants.size();
//    for (int i = 0; i < endIndexHolder; i++) {
//        auto& upperdata = primeImplicants[i];
//
////        for (int elem : upperdata.mintermsIncluded) {
////            for (int j = i + 1; j < endIndexHolder; j++) {
////                const auto& lowerdata = primeImplicants[j];
////
////                for (int element : lowerdata.mintermsIncluded) {
////                    if (element == elem) {
////                        //in here it is true;
////                    }
////                }
////                //in here it is false;
////
////            }
////        }
//
//        bool isEPI = false;
//        for (int elem : upperdata.mintermsIncluded) {
//            outer_loop:  // Label for the outer loop
//            bool found = false;
//            for (int j = i + 1; j < endIndexHolder; j++) {
//                const auto& lowerdata = primeImplicants[j];
//
//                for (int element : lowerdata.mintermsIncluded) {
//                    if (element == elem) {
//                        // Condition is true, break out of both inner loops and move to next iteration of outer loop
//                        found = true;
//                        goto outer_loop;
//                    }
//                }
//            }
//            if(!found){
//                //break and mark as EPI
//                isEPI = true;
//                break;
//            }
//            // Continue here after breaking out of the loops (in like goto outer_loop)
//        }
//
//        upperdata.isEssential = isEPI;
//
//
//
//
//    }

//    int endIndexHolder = primeImplicants.size();
//    for (int i = 0; i < endIndexHolder; i++) {
//        auto& upperdata = primeImplicants[i];
//
//        bool isEPI = false;
//        for (int elem : upperdata.mintermsIncluded) {
//            bool found = false;
//            for (int j = i + 1; j < endIndexHolder; j++) {
////                if (j >= primeImplicants.size()) {
////                    // Handle out-of-range error
////                    // You might want to log an error or handle this case differently
////                    break;
////                }
//                auto& lowerdata = primeImplicants[j];
//                for (int element : lowerdata.mintermsIncluded) {
//                    if (element == elem) {
//                        found = true;
//                        if(lowerdata.isEssential == -1)
//                            lowerdata.isEssential = 0;
//                        break;  // Exit the inner loop early since we found a match
//                    }
//                }
//                if (found) {
//                    break;  // Exit the outer loop early since we found a match
//                }
//            }
//            if (!found) {
//                isEPI = true;
//                break;  // Mark as EPI and exit the outer loop
//            }
//        }
//
//        if(upperdata.isEssential == -1)
//            upperdata.isEssential = isEPI;
//        if (isEPI && upperdata.isRequired == -1)
//            upperdata.isRequired = isEPI;
//    }

//*****************************
    int endIndexHolder = primeImplicants.size();
    for (int i = 0; i < endIndexHolder; i++) {
        auto& upperdata = primeImplicants[i];

        bool isEPI = false;
        for (int elem : upperdata.mintermsIncluded) {
            bool found = false;
            for (int j = 0; j < endIndexHolder; j++) {
                if(i == j)
                    continue;
                const auto& lowerdata = primeImplicants[j];
                for (int element : lowerdata.mintermsIncluded) {
                    if (element == elem) {
                        found = true;
//                        if(lowerdata.isEssential == -1)
//                            lowerdata.isEssential = 0;
                        break;  // Exit the inner loop early since we found a match
                    }
                }
                if (found) {
                    break;  // Exit the outer loop early since we found a match
                }
            }
            if (!found) {
                isEPI = true;
                break;  // Mark as EPI and exit the outer loop
            }
        }

        if(upperdata.isEssential == -1)
            upperdata.isEssential = isEPI;
        if (isEPI && upperdata.isRequired == -1)
            upperdata.isRequired = isEPI;
    }

}

void QuineMcCluskey::ApplyPetricsMethod() {
    // going for Petrics method if needed. checking if there is any uncovered Minterm, this should suffice right?
    if(uncoveredMintermReimained()){
        //use index if unessential PI as it's label
        // create a vector that holds boolean expression and then define methods to do that bool aljebra


        //looping over reamining minterms to create bool expression:
        std::vector<std::vector<int>> andExpressions;
        for (const auto & elem: Minterms)
            if (!elem.isCovered){
                //loop over primeImplicants and push back the indexes into the shit
                int hold = int(primeImplicants.size());
                std::vector<int> temp;
                for (int i = 0; i < hold; ++i)
                    if(!primeImplicants[i].isEssential)
                        for (const auto & minterm : primeImplicants[i].mintermsIncluded)
                            if (elem.minterm == minterm){
                                temp.push_back(i);
                                break;
                            }
                andExpressions.push_back(temp);
//                std::cout << "or elem : [ ";
//                for (int t: temp) {
//                    std::cout << t << " ";
//                }
//                std::cout << "]" << std::endl;
            }

        //by now, the initial P has been formed (andExpression). now just we need to simplify it and convert it into SOP form





    }
}

void QuineMcCluskey::printSimplifiedExpression() {
    // Implement printing the simplified Boolean expression
}

int QuineMcCluskey::detectBitsCount() {
    int num = Minterms[numMinterms - 1].minterm;

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

//void QuineMcCluskey::buildMinterms(int* inputArray){
//    for (int i = 0; i < numMinterms; i++){
//        Minterms.push_back({inputArray[i], false});
//    }
//}

void QuineMcCluskey::buildMinterms(int* inputArray){
    std::sort(inputArray, inputArray + numMinterms);  // Sort the input array first
    for (int i = 0; i < numMinterms; i++){
        Minterms.push_back({inputArray[i], false});
    }
}


bool QuineMcCluskey::hasCommonElement(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    for (int elem : vec1) {
        if (std::find(vec2.begin(), vec2.end(), elem) != vec2.end()) {
            return true;
        }
    }
    return false;
}

void QuineMcCluskey::updatedCoveredMinterms(){
    for(auto& minterm : Minterms){
        if(!minterm.isCovered){//should it be like this?
            bool isCovered = false;
            for (const auto& primeI : primeImplicants){
                if(primeI.isRequired == 1){
                    for (int elem : primeI.mintermsIncluded){
                        if(minterm.minterm == elem){
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

bool QuineMcCluskey::uncoveredMintermReimained(){
    for (const auto & elem: Minterms)
        if (!elem.isCovered)
            return true;
    return false;
}

// QuineMcCluskey::~QuineMcCluskey() {
//   delete[] inputArray;
// }
// void QuineMcCluskey::setup() {
//   // Example method implementation
//   // Perform setup tasks
// }
