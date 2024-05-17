// QuineMcCluskey.h

#ifndef QuineMcCluskey_h
#define QuineMcCluskey_h

// #include <Arduino.h>
// #include <avr/pgmspace.h>

class QuineMcCluskey {
private:
  int* inputArray;
  int arrayLength;
  int indicator;

public:
  QuineMcCluskey();
  QuineMcCluskey(int* inputArray, int length, int indicator);  // Constructor
  // ~QuineMcCluskey(); // Destructor

  void setup(); // Example method
  void solve(); // Example method

  int detectBitsCount();
};

#endif
