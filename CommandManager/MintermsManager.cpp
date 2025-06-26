// MintermsManager.cpp

#include "MintermsManager.h"

int minterms[MAX_TERMS] = {-1}; 
int numMinterms = 0;    
int dontCares[MAX_TERMS] = {-1}; 
int numDontCares = 0;   
bool isMaxterm = false;      

void addMinterm(int value) {
  if (numMinterms < MAX_TERMS) {
    minterms[numMinterms++] = value;
  }
}
void addDontCare(int value) {
  if (numDontCares < MAX_TERMS) {
    minterms[numDontCares++] = value;
  }
}


void clearMinterms() {
  for (int i = 0; i < MAX_TERMS; i++) {
    minterms[i] = -1; 
  }
  numMinterms = 0; 
}
void clearDontCares() {
  for (int i = 0; i < MAX_TERMS; i++) {
    dontCares[i] = -1; 
  }
  numDontCares = 0; 
}


//These are completely unnecesary, these are handled in the other side.
void removeDuplicates() {
  for (int i = 0; i < numMinterms; i++){
    for (int j = i + 1; j < numMinterms; j++){
      if(minterms[i] == minterms[j] && minterms[i] >= 0){
        minterms[j] = -1;
      }
    }
  }

  int positiveStartIndex = 0;
  for (int i = 0; i < numMinterms; i++) {
    if (minterms[i] >= 0) {
      minterms[positiveStartIndex++] = minterms[i];
    }
  }

  numMinterms = positiveStartIndex;
}

void sortMinterms() {
  for (int i = 1; i < numMinterms; i++) {
    int key = minterms[i];
    int j = i - 1;
    while (j >= 0 && minterms[j] > key) {
      minterms[j + 1] = minterms[j];
      j = j - 1;
    }
    minterms[j + 1] = key;
  }
}

bool mintermsNotEmpty(){
  if(numMinterms > 0){
    return true;
  }

  return false;
}

String packUpDataToString(){
  String data = "";
  data += "mainTerms: ";
  for (int i = 0; i < numMinterms; i++) {
    data += String(minterms[i]);
    if (i < numMinterms - 1) {
      data += ", ";
    }
  }
  data += "; numMainTerms: " + String(numMinterms);

  data += "; dontCares: ";
  for (int i = 0; i < numDontCares; i++) {
    data += String(minterms[i]);
    if (i < numDontCares - 1) {
      data += ", ";
    }
  }
  data += "; numDontCares: " + String(numDontCares);
  data += "; isMaxTerm: " + String(isMaxterm);

  return data;
}