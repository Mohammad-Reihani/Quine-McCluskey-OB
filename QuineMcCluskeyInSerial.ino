#include <SoftwareSerial.h>

// SoftwareSerial pins on ESP8266 NodeMCU (adjust pins as needed)
SoftwareSerial esp8266Serial(D5, D6); // RX (GPIO14), TX (GPIO12)

bool incomingDone = false;
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
    incomingDone = true;
  }

  if(incomingDone){
    delay(1000);
    // esp8266Serial.print("s"); //this is the start mark
    esp8266Serial.print("hello boy!");
    // esp8266Serial.print("q"); //this is the end mark
    incomingDone = false;
  }


}
