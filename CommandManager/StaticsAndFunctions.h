#ifndef StaticsAndFunctions_h
#define StaticsAndFunctions_h

#include <Arduino.h>

extern String commands[];
extern int cmdSize;

int findStringInCommands(const String& target);

#endif
