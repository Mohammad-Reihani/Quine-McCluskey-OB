// MintermsManager.h

#ifndef MintermsManager_h
#define MintermsManager_h

#include <Arduino.h>

const int MAX_TERMS = 100;
extern int minterms[MAX_TERMS];
extern int numMinterms;
extern int dontCares[MAX_TERMS];
extern int numDontCares;
extern bool isMaxterm;

void addMinterm(int value);
void clearMinterms();
void removeDuplicates();
void sortMinterms();
bool mintermsNotEmpty();
String packUpDataToString();

void addDontCare(int value);
void clearDontCares();
#endif
