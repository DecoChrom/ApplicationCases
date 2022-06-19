int p1 = 3; //connect display , only odd numbers work
int p2 = 5; //connect display , only odd numbers work
int LEDpin = 2; //connect LED
int Value;
int state;
int analogVal = 0; //stores the value that comes from battery
float volt = 0; //stores the calculated voltage value 

void setup() {
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);
  state = 2;
  updateDisplayState(state); //start with display off
}

void loop() {
  if (Serial.available()>0) {
    Value = Serial.read();

    if (Value == '0') {
       state = 0;
       updateDisplayState(state); //switch display to red
    }
    else if (Value == '1') {
       state = 1;
       updateDisplayState(state); //switch display to blue
    }
    else if (Value == '2') {
      state = 2;
      updateDisplayState(state); //switch display off
    }
  }
//  analogVal = analogRead(A3);//reads the analog pin value
//  volt = 0.0049 * analogVal;//calculation to convert to voltage
//  if(volt <= 1.5){//blink when battery is low
//    digitalWrite(LEDpin, HIGH);
//    delay(100);
//    digitalWrite(LEDpin, HIGH);
//    delay(100);
//  }
//  else if(volt > 1.5 && volt < 3.7){//checks to see if calculated voltage is between 1.2 and 1.5 
//  }
//  else if(volt >= 3.7){//light up when battery is charged
//    digitalWrite(LEDpin, HIGH);
//  }
  delay(10);
}

void updateDisplayState (int onOff) {
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  int voltValue = 120;
  switch (onOff) {
    case 0:
      analogWrite(p1, voltValue);
      digitalWrite(p2, LOW);
      break;
    case 1:
      digitalWrite(p1, LOW);
      analogWrite(p2, voltValue);
      break;
    case 2:
      digitalWrite(p1, LOW);
      digitalWrite(p2, LOW);
      break;
    default:
      pinMode(p1, INPUT); // high impedence
      pinMode(p2, INPUT);
      break;
  }
}
