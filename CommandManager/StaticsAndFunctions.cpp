// CommandsManager.cpp

#include "StaticsAndFunctions.h"

String commands[] = {"go", "done", "reset", "clear"};
int cmdSize = 4;

int findStringInCommands(const String& target) {
  for (int i = 0; i < cmdSize; i++) {
    if (target == commands[i]) {
      return i;
    }
  }
  return -1;
}
