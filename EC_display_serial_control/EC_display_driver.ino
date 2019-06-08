/*  Electrochromic display driver / relay
    Extracts first character from a string to set electrochromic display state
    Remainder of string is forwarded to next driver / relay in a chain
    Target: Attiny85

    Ashley Colley, University of Lapland 21.5.19

    Control string contents are
    "0" = display state A (HIGH / LOW)
    "1" = display state B (LOW / HOGH)
    "2" = Both display connetions LOW
    "x" = Display connections high impedence (maximise EC display memory)

     Example string for 7 display segments "1011x0x"
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX


#define NUM_SEGMENTS  1 // how many segments in this display

void setup() {
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);


  startTest();
}

void loop() { // run over and over

  if (mySerial.available()) {
    String temp = mySerial.readString();
    String newString = temp.substring(NUM_SEGMENTS); // remainder after removed to pass to next node
    delay(5); // AC adds
    mySerial.println(newString); // pass to next node

    switch (temp.charAt(0)) {
      case '0':
        pinMode(3, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        break;
      case '1':
        pinMode(3, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        break;
      case '2':
        pinMode(3, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        break;
      default:
        pinMode(3, INPUT); // set output to high impedence
        pinMode(4, INPUT);
        break;
    }
  }
}

void startTest(){
        pinMode(3, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
     delay(2000);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
     delay(2000);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
  
  }
