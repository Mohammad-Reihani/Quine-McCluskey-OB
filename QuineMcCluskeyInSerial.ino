#include <SoftwareSerial.h>
#include "QuineMcCluskey.h"

// SoftwareSerial pins on ESP8266 NodeMCU (adjust pins as needed)
SoftwareSerial esp8266Serial(D5, D6);  // RX (GPIO14), TX (GPIO12)

bool incomingDone = false;
const int maxMinterms = 100;
int minterms[maxMinterms];
int numMinterms;
int indicator;

QuineMcCluskey testQMC;

void parseReceivedMessage(String& receivedString) {
  // Find the indices of key substrings
  int startIdx = receivedString.indexOf("minterms: ") + 10;
  int endIdx = receivedString.indexOf("; numMinterms:");
  int numMintermsStartIdx = endIdx + 14;
  int numMintermsEndIdx = receivedString.indexOf("; ", numMintermsStartIdx);
  int lastIntValueStartIdx = numMintermsEndIdx + 2;

  // Check if indices are valid
  if (startIdx == -1 || endIdx == -1 || numMintermsStartIdx == -1 || numMintermsEndIdx == -1 || lastIntValueStartIdx == -1) {
    Serial.println("Error: Invalid message format");
    return;
  }

  // Extract substrings for minterms, numMinterms, and last integer value
  String mintermsStr = receivedString.substring(startIdx, endIdx);
  String numMintermsStr = receivedString.substring(numMintermsStartIdx, numMintermsEndIdx);
  String lastIntValueStr = receivedString.substring(lastIntValueStartIdx);

  // Convert strings to integers
  numMinterms = numMintermsStr.toInt();
  indicator = lastIntValueStr.toInt();

  // Extract and store minterms array elements
  int commaIdx = mintermsStr.indexOf(',');
  int arrayIdx = 0;
  while (commaIdx != -1) {
    String minterm = mintermsStr.substring(0, commaIdx);
    minterms[arrayIdx] = minterm.toInt();
    mintermsStr = mintermsStr.substring(commaIdx + 1);  // Skip ","
    commaIdx = mintermsStr.indexOf(',');
    arrayIdx++;
  }
  // Last element (no comma after)
  minterms[arrayIdx] = mintermsStr.toInt();
}



void setup() {
  Serial.begin(19200);
  esp8266Serial.begin(9600);
  Serial.println("Testing the ESP8266 communication");
}


void loop() {
  if (esp8266Serial.available()) {
    String receivedString = esp8266Serial.readStringUntil('\n');
    parseReceivedMessage(receivedString);
    testQMC = QuineMcCluskey(minterms, numMinterms, indicator);
    testQMC.solve();

    // Serial.println("Grouped Terms:");
    // for (const auto& term : testQMC.groupedTerms) {
    //     Serial.print("(");
    //     Serial.print(term.first); // Print the minterm
    //     Serial.print(", ");
    //     Serial.print(term.second); // Print the group number
    //     Serial.println(")");
    // }

    Serial.println("Grouped Terms:");
    for (const auto data : testQMC.groupedTerms) {
      Serial.print("Minterms Included: [");
      for (const auto minterm : data.mintermsIncluded) {
        Serial.print(minterm);
        Serial.print(" ");
      }
      Serial.print("] Deleted Args: [");
      for (const auto arg : data.deletedArgs) {
        Serial.print(arg);
        Serial.print(" ");
      }
      Serial.print("] Stage: [");
      Serial.print(data.stage);
      Serial.print("] groupFromTop: [");
      Serial.print(data.groupFromTop);
      Serial.print("] isPI: ");
      Serial.println(data.isPI);
    }

    incomingDone = true;
  }

  if (incomingDone) {
    delay(1000);

    esp8266Serial.print("sample response");

    incomingDone = false;
  }
}
