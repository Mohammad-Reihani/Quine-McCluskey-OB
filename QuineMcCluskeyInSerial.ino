#include <SoftwareSerial.h>
#include "QuineMcCluskey.h"

SoftwareSerial esp8266Serial(D5, D6);  // RX (GPIO14), TX (GPIO12)

const int maxTerms = 100;
int mainTerms[maxTerms];
int numMainTerms;
int dontCareTerms[maxTerms];
int numDontCareTerms;

bool isMaxTerm;
bool incomingDone = false; //indicates if the data has come and the response has been sent

QuineMcCluskey qmc;
String response = "";


// Received info from arduino parser
void parseReceivedMessage(String& receivedString) {
    // Find the indices of key substrings
    int startIdx = receivedString.indexOf("mainTerms: ") + 11;
    int endIdx = receivedString.indexOf("; numMainTerms:");
    int numMainTermsStartIdx = endIdx + 15;
    int numMainTermsEndIdx = receivedString.indexOf("; dontCares:");
    int dontCaresStartIdx = numMainTermsEndIdx + 13;
    int dontCaresEndIdx = receivedString.indexOf("; numDontCares:", dontCaresStartIdx);
    int numDontCareTermsStartIdx = dontCaresEndIdx + 15;
    int numDontCareTermsEndIdx = receivedString.indexOf("; isMaxTerm:", numDontCareTermsStartIdx);
    int isMaxTermStartIdx = numDontCareTermsEndIdx + 13;

    // Check if indices are valid
    if (startIdx == -1 || endIdx == -1 || numMainTermsStartIdx == -1 || numMainTermsEndIdx == -1 || dontCaresStartIdx == -1 ||
        dontCaresEndIdx == -1 || numDontCareTermsStartIdx == -1 || numDontCareTermsEndIdx == -1 || isMaxTermStartIdx == -1) {
        Serial.println("Error: Invalid message format");
        return;
    }

    // Extract substrings for mainTerms, numMainTerms, dontCareTerms, numDontCareTerms, and isMaxTerm
    String mainTermsStr = receivedString.substring(startIdx, endIdx);
    String numMainTermsStr = receivedString.substring(numMainTermsStartIdx, numMainTermsEndIdx);
    String dontCaresStr = receivedString.substring(dontCaresStartIdx, dontCaresEndIdx);
    String numDontCareTermsStr = receivedString.substring(numDontCareTermsStartIdx, numDontCareTermsEndIdx);
    String isMaxTermStr = receivedString.substring(isMaxTermStartIdx);

    isMaxTermStr.trim();
    Serial.print("isMaxTermStr: '");
    Serial.print(isMaxTermStr);
    Serial.print("'");
    Serial.print(" with length ");
    Serial.println(isMaxTermStr.length());
    // Convert strings to integers
    numMainTerms = numMainTermsStr.toInt();
    numDontCareTerms = numDontCareTermsStr.toInt();
    isMaxTerm = isMaxTermStr.equals("0") ? false : true;

    // Helper function to split a comma-separated string into an array of integers
    auto splitToArray = [](const String& str, int* arr) {
        int startIndex = 0;
        int endIndex = str.indexOf(',');
        int idx = 0;
        while (endIndex != -1) {
            arr[idx++] = str.substring(startIndex, endIndex).toInt();
            startIndex = endIndex + 1;
            endIndex = str.indexOf(',', startIndex);
        }
        arr[idx] = str.substring(startIndex).toInt(); // Last element
    };

    // Extract and store mainTerms and dontCareTerms array elements
    splitToArray(mainTermsStr, mainTerms);
    splitToArray(dontCaresStr, dontCareTerms);

    // Debug output to check the extracted values (optional)
    Serial.print("mainTerms: ");
    for (int i = 0; i < numMainTerms; ++i) {
        Serial.print(mainTerms[i]);
        if (i < numMainTerms - 1) Serial.print(", ");
    }
    Serial.println();

    Serial.print("numMainTerms: ");
    Serial.println(numMainTerms);

    Serial.print("dontCareTerms: ");
    for (int i = 0; i < numDontCareTerms; ++i) {
        Serial.print(dontCareTerms[i]);
        if (i < numDontCareTerms - 1) Serial.print(", ");
    }
    Serial.println();

    Serial.print("numDontCareTerms: ");
    Serial.println(numDontCareTerms);

    Serial.print("isMaxTerm: ");
    Serial.println(isMaxTerm ? "true" : "false");
}

void setup() {
  Serial.begin(19200);
  esp8266Serial.begin(9600);
  Serial.println("ESP8266 Connected via serial"); //just a debug message
}


void loop() {
  if (esp8266Serial.available()) {
    String receivedString = esp8266Serial.readStringUntil('\n');
    parseReceivedMessage(receivedString);
    qmc = QuineMcCluskey(mainTerms, numMainTerms, isMaxTerm);
    qmc.solve();

    //********************************************************************************************
    // Printing some stuff. some extra info, to see better thing, view the written class demo on your local machine.
    // Nothing that matters so much, but if you want to view this in serial too, just in case. right?
    Serial.println("-------------------------------------------------------------------");
    Serial.println("The function is now simplified, getting ready to display results : ");
    Serial.println("-------------------------------------------------------------------");
    Serial.println("Grouped Terms:");//Showing the whole simplification table, can you print it vertically?
    for (const auto& data : qmc.getGroupedTerms()) {
        Serial.print("MainTerms Included: [");
        for (const auto& minterm : data.termsIncluded) {
            Serial.print(minterm);
            Serial.print(" ");
        }
        Serial.print("] Deleted Args: [");
        for (const auto& arg : data.deletedArgs) {
            Serial.print(arg);
            Serial.print(" ");
        }
        Serial.print("] stage: ");
        Serial.print(data.stage);
        Serial.print(" isPI: ");
        Serial.println(data.isPI);
    }
    Serial.println("-------------------------------------------------------------------");
    Serial.println("Prime Implicants:");//Show prime implicants with details
    for (const auto& data : qmc.getPrimeImplicants()) {
        Serial.print("MainTerms Included: [");
        for (const auto& minterm : data.termsIncluded) {
            Serial.print(minterm);
            Serial.print(" ");
        }
        Serial.print("] Deleted Args: [");
        for (const auto& arg : data.deletedArgs) {
            Serial.print(arg);
            Serial.print(" ");
        }
        Serial.print("] isEssential: ");
        Serial.print(data.isEssential);
        Serial.print(" isRequired: ");
        Serial.println(data.isRequired);
    }
    Serial.println("-------------------------------------------------------------------");
    Serial.println("MainTerms status:");//Showing the status of mainTerms if they're covered or not.
    for (const auto& data : qmc.getMainTerms()) {
        Serial.print("term : ");
        Serial.print(data.term);
        Serial.print(" | is covered : ");
        Serial.println(data.isCovered);
    }
    Serial.println("-------------------------------------------------------------------");
    Serial.println("Simplified Expression : ");
    Serial.println(String(qmc.getStringExpression().c_str()));

    //********************************************************************************************


    //Getting and sending the response to display
    response = String(qmc.getStringExpression().c_str());
    response += '*'; //this is an end mark
    Serial.println(response);

    incomingDone = true;
  }

  if (incomingDone) {
    delay(1000);

    esp8266Serial.print(response);
    response = "";
    incomingDone = false;
  }
}
