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

class QuineMcCluskey {
private:
  int* minterms;
  int numMinterms;
  int indicator;
  int numInputs;



  void groupMinterms();
  void generatePrimeImplicants();
  void findEssentialPrimeImplicants();
  void simplifyBooleanExpression();
  void printSimplifiedExpression();
  int detectBitsCount();

public:
  QuineMcCluskey();
  QuineMcCluskey(int* inputArray, int length, int indicator);
  
  void solve();
};

#endif
