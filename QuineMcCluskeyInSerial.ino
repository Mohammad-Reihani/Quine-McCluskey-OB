#include <SoftwareSerial.h>
#include "QuineMcCluskey.h"

// SoftwareSerial pins on ESP8266 NodeMCU (adjust pins as needed)
SoftwareSerial esp8266Serial(D5, D6);  // RX (GPIO14), TX (GPIO12)

bool incomingDone = false;
const int maxMinterms = 100;
int minterms[maxMinterms];
int numMinterms;
int indicator;

// QuineMcCluskey testQMC;

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
    // char c = esp8266Serial.read();
    // if (c == 's') { // Start marker
    //   String message = "";
    //   while (c != 'q') { // Read until end marker is received
    //     c = esp8266Serial.read();
    //     if (c != 'q') {
    //       message += c;
    //     }
    //   }
    //   Serial.println("Received message: " + message);
    //   incomingDone = true;
    // }
    String receivedString = esp8266Serial.readStringUntil('\n');
    Serial.println("Received message: " + receivedString);
    parseReceivedMessage(receivedString);
    Serial.println("the shit got parsed");

    Serial.println(indicator);
    Serial.println(numMinterms);
    for (int i = 0; i < numMinterms; i++){
      Serial.print(minterms[i]);
      Serial.print(" ");
    }
    Serial.println("");


    QuineMcCluskey testQMC(minterms, numMinterms, indicator);  // Assignment operator called
    Serial.println("the object is now made");
    Serial.println(testQMC.detectBitsCount());

    incomingDone = true;
  }

  if (incomingDone) {
    delay(1000);
    // esp8266Serial.print("s"); //this is the start mark
    // esp8266Serial.print(testQMC.detectBitsCount());
    esp8266Serial.print("Temp");
    // esp8266Serial.print("q"); //this is the end mark
    incomingDone = false;
  }
}
