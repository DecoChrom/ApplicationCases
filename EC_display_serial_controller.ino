/* Electrochromic display controller
    Sends string sequences to control EC display chain using SW serial
    Ashley Colley, University of Lapland 21.5.19
*/


#include <SoftwareSerial.h>
// define sw serial pins
SoftwareSerial mySerial(4, 5); // RX, TX // On nrf52 this is A2 and A3
//SoftwareSerial mySerial(2, 16); // RX, TX
long unsigned startTime = millis();

// Button input pins
int buttonPins[] = {16, 15, 7, 11};
int numButtons = 4;

// each string is a pattern sequence 1 = on, 0 = off
// ### REMEMBER TO PUT A COMMA AT THE END ###
// grows one at a time
String s0 = "1000000000,1100000000,1110000000,1111000000,1111100000,1111110000,1111111000,1111111100,1111111110,1111111111,";
// single segment moving - can be repeated
String s1 = "1000010000,0100001000,0010000100,0001000010,0000100001,";
// togling all - can be repeated
String s2 = "1010101010,0101010101,";
// also add random

void setup() {
  delay(10);
  Serial.begin(9600); // HW to USB serial monitor
  delay(10);
  mySerial.begin(9600); // to attiny
  delay(10);

  //define the user buttons
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(A0));  

}


void loop() {

  // this is for testing from the console
  if (Serial.available()) {
    String temp = Serial.readString();
    Serial.print("TX ");
    Serial.println(temp); // local echo
    mySerial.println(temp); // read from keyboard and send out in virtual
  }


  // read the buttons
  int selectedItem = -1;
  for (int i = 0; i < numButtons; i++) {
    if ( digitalRead(buttonPins[i]) == LOW) {
     selectedItem = i;
      Serial.print("Button_press ");
      Serial.println(i);
    }
  }

  // 1st version is blocking - later make interupts possible
  int numberOfCycles = 1;
  switch (selectedItem) {
    case 0:
      setDisplay (s0, 2000, 1);
      break;
    case 1:
      setDisplay (s1, 2000, 2);
      break;
    case 2:
      setDisplay (s2, 2000, 4);
      break;
    case 3:
      setRandom (2000, 10);
      break;

  }


  //   this is loopback just for testing
  delay(5);        // delay in between reads for stability was 5
  if (mySerial.available()) {
    Serial.print("RX ");
    Serial.println(mySerial.readString()); // to serial monitor
  }

}


void setDisplay (String inputString, int delayTime, int cycles) {
  int lastComma;
  String commandString;
  Serial.print("string len = ");
  Serial.println(inputString.length());

  for (int j = 0; j < cycles; j++) { // number of repeats
    lastComma = 0;

    for (int i = 0; i < inputString.length(); i++) {
      if (inputString[i] == ',') {
        // extract the command string
        commandString = inputString.substring(lastComma, i);
        Serial.print("TX ");
        Serial.println(commandString); // local echo
        mySerial.println(commandString);
        lastComma = i + 1;
        delay(delayTime); // in milliseconds
      } //if
    } //i
  } //j
}


void setRandom (int delayTime, int cycles) {

  for (int i = 0; i < cycles; i++) {
    String commandString = "0000000000"; // clear it
    for (int j = 0; j < 5; j++) { // build a random string of 1s and 0s
      commandString.setCharAt(random(commandString.length()), '1');
    }

    Serial.print("TX ");
    Serial.println(commandString); // local echo
    mySerial.println(commandString);
    delay(delayTime); // in milliseconds
  } //for
}
