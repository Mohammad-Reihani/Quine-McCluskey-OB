#include <PS2Keyboard.h>
#include <LCD_I2C.h>
#include <SoftwareSerial.h>

#include "MintermsManager.h"
#include "StaticsAndFunctions.h"


SoftwareSerial esp8266Serial(0, 1);  // RX, TX
PS2Keyboard keyboard;
LCD_I2C lcd(0x27, 16, 2);

const int DataPin = 2;
const int IRQpin = 3;

int scrollIndicator = 0;

String input;
String receivedString{};
String serialResponse;

bool inputDone = false;
bool isUserEnteringMinMaxTerms = false;
bool isDataSent = false;
bool mainTermsEntered = false;
bool isUserEnteringDontCares = false;
bool askedForDontCares = false;
bool askedForIsMaxTerm = false;
bool askingForIsMaxTerm = false;
bool isShowingResult = false;



/*function are in here*/
void (*resetFunc)(void) = 0;

void clearLine(int line) {
  lcd.setCursor(0, line);
  lcd.print("                ");
  lcd.setCursor(0, line);
}
void scrollResultToRight() {
  if (receivedString.length() <= 16) {
    // If the string length is 16 or less, no need to scroll
    return;
  }

  scrollIndicator = min(scrollIndicator + 1, receivedString.length() - 16);

  clearLine(1);
  lcd.print(receivedString.substring(scrollIndicator, scrollIndicator + 16));
}
void scrollResultToLeft() {
  if (receivedString.length() <= 16) {
    // If the string length is 16 or less, no need to scroll
    return;
  }

  scrollIndicator = max(scrollIndicator - 1, 0);

  clearLine(1);
  lcd.print(receivedString.substring(scrollIndicator, scrollIndicator + 16));
}


// Command case handlers:
void handleCase0(bool& skip) {
  //go command
  isUserEnteringMinMaxTerms = true;
  isUserEnteringDontCares = false;
  askingForIsMaxTerm = false;
  skip = true;
  isShowingResult = false;
  lcd.clear();
  lcd.noAutoscroll();
  lcd.setCursor(0, 0);
  lcd.print(F("Enter MainTerm :"));
  clearLine(1);
}
void handleCase1(bool& skip) {
  //done command
  isUserEnteringMinMaxTerms = false;
  isUserEnteringDontCares = false;
  askingForIsMaxTerm = false;
  isShowingResult = false;

  if (!mintermsNotEmpty()){
    lcd.clear();
    clearLine(0);
    lcd.print("No MainTerm are");
    clearLine(1);
    lcd.setCursor(0, 1);
    lcd.print("available!use go");
  }
  else if (mintermsNotEmpty() && askedForDontCares && askedForIsMaxTerm) {
    lcd.clear();
    lcd.noAutoscroll();
    lcd.setCursor(0, 0);
    lcd.print("Processing...");

    removeDuplicates();
    sortMinterms();

    //**************************************************************************

    String data = packUpDataToString();
    Serial.println(data);
    esp8266Serial.print(data);

    clearLine(0);
    lcd.print("Data Sent. Wait");
    clearLine(1);
    lcd.print("for response ...");
    isDataSent = true;
    skip = true;
    //**************************************************************************

    //Clearing minterms for next use!
    clearMinterms();
    clearDontCares();
    askedForDontCares = false;
    askedForIsMaxTerm = false;
  }
  else if (!askedForDontCares){
    //woke the case that gets Don't cares;
    handleCase4(skip);
  }
  else if (!askedForIsMaxTerm){
    //woke the case that ask for is maxterm;
    handleCase5(skip);
  }
}
void handleCase2(bool& skip) {
  isShowingResult = false;
  //reset command
  resetFunc();
}
void handleCase3(bool& skip) {
  //clear command
  isShowingResult = false;
  lcd.clear();
  clearLine(0);
  lcd.print("clearing datas...");

  clearMinterms();
  clearDontCares();
  isMaxterm = false;

  delay(1000);
  clearLine(0);
  lcd.print("datas cleared!");
  delay(1000);
  lcd.clear();


  handleCase0(skip);
}
void handleCase4(bool& skip) {
  //handle the case to enter don't cares:
  isUserEnteringMinMaxTerms = false;
  isUserEnteringDontCares = true;
  askedForDontCares = true;
  askingForIsMaxTerm = false;
  skip = true;
  isShowingResult = false;
  lcd.clear();
  lcd.noAutoscroll();
  lcd.setCursor(0, 0);
  lcd.print(F("Enter DontCare :"));
  clearLine(1);
}
void handleCase5(bool& skip) {
  //handle the case to enter don't cares:
  isUserEnteringMinMaxTerms = false;
  isUserEnteringDontCares = false;
  askingForIsMaxTerm = true;
  isShowingResult = false;
  
  skip = true;
  lcd.clear();
  lcd.noAutoscroll();
  lcd.setCursor(0, 0);
  lcd.print(F("Entred MaxTerms?"));
  clearLine(1);
}


void setup() {
  esp8266Serial.begin(9600);
  keyboard.begin(DataPin, IRQpin, PS2Keymap_US);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();


  lcd.setCursor(0, 0);
  lcd.print(F("ENETER a command"));
  lcd.setCursor(0, 1);
  lcd.print(F("    to start"));
}

void loop() {
  if (!isDataSent && keyboard.available()) {
    int unicode = keyboard.readUnicode();
    char c = unicode;

    if (unicode != 0) {
      if (unicode == 13) {
        inputDone = true;
      } else if (unicode == 127 && input.length() > 0) {
        input.remove(input.length() - 1);
      } else if (unicode == 21){
        // right arrow
        if (isShowingResult){
          // scroll the result to the right
          scrollResultToRight();
        }
      } else if (unicode == 8){
        // left arrow
        if (isShowingResult){
          // scroll the result to the left
          scrollResultToLeft();
        }
      } else {
        input += c;
      }
    }

    //Showing real time input in line 1 (2) | except when showing result! (to be able to scroll left and righ)
    // if((isUserEnteringMinMaxTerms || isUserEnteringDontCares || askingForIsMaxTerm) && !isShowingResult){
    if(!isShowingResult){
      clearLine(1);
      lcd.print(input);
    }
  }

  if (!isDataSent && inputDone) {
    int response = findStringInCommands(input);
    bool skip = false;

    switch (response) {
      case 0:
        handleCase0(skip);
        break;
      case 1:
        handleCase1(skip);
        break;
      case 2:
        handleCase2(skip);
        break;
      case 3:
        handleCase3(skip);
        break;
      default:
        break;
    }


    if (!skip) {
      if (isUserEnteringMinMaxTerms || isUserEnteringDontCares) {
        clearLine(1);
        int integerValue = input.toInt();

        if (input == String(integerValue) && integerValue >= 0) {
          if (isUserEnteringMinMaxTerms)
            addMinterm(integerValue);
          else if (isUserEnteringDontCares)
            addDontCare(integerValue);

          lcd.print("ok ");
          lcd.print(integerValue);
          delay(300);
          clearLine(1);
        } else {
          lcd.print("invalid input!");
          delay(1000);
          clearLine(1);
        }


      } else if (askingForIsMaxTerm) {
        if (input == "0")
          isMaxterm = false;
        else
          isMaxterm = true;

        askingForIsMaxTerm = false;
        askedForIsMaxTerm = true;
        handleCase1(skip);
      }
    }


    //Crusial!
    inputDone = false;
    input = "";
  }


  // This function if for receiving the answer of simplification
  if (isDataSent && esp8266Serial.available()) {
    receivedString = esp8266Serial.readStringUntil('*');

    clearLine(0);
    lcd.print("Got the response");
    clearLine(1);
    lcd.print("prep for display");

    //calculations
    delay(1000);



    lcd.clear();
    clearLine(0);
    lcd.print("the answer is:");

    isShowingResult = true;
    scrollIndicator = 0;

    clearLine(1);
    clearLine(1);
    lcd.print(receivedString.substring(0, 16));
    isDataSent = false;  //and maybe reset the whole thing;
  }
}
