#include <Arduino.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFunLIS3DH.h>
#include <SparkFunSX1509.h> // Includes I/O Extender

// Parameters for stepcounter
LIS3DH accel;
SX1509 io;
int accelTimeOut = 100;
int lastChangeAccelTimeOut = 0;
float threshold=1.1;
float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xaccl = 0;
float yaccl = 0;
float zaccl = 0;
float totvect = 0;
float xavg;
float yavg;
float zavg;
int steps,flag=0;

//Parameters for EC-display
//Pins for toe-box
const int displayPin0 = 6;
const int displayPin1 = 2;
const int displayPin2 = 7;
const int displayPin3 = 4;
const int displayPin4 = 5;
const int displayPinCE = 3;
//Pins for pronation display
const int displayPinPronationIn0 = 8;
const int displayPinPronationIn1 = 9;
const int displayPinPronationOut0 = 10;
const int displayPinPronationOut1 = 11;
//Pins for wear-and-tear side display
const int displayPinWT0 = 12;
const int displayPinWT1 = 13;
bool side = true;
float voltage = 1.2;
unsigned long lastChange = 0;

//Parameters for pronation detection
const int piezoSensor0 = 26; 
const int piezoSensor1 = 25;
int sensorReading0 = 0;     
int sensorReading1 = 0;
int sensorMax0 = 0;
int sensorMax1 = 0;
int pronation = 0;
bool flagLeft = false;
bool flagRight = false;

// Defining the functions
void setDisplayPower(int pin, float voltage);
void setDisplayWTPower(bool side, float voltage);
void setDisplayPronationPower(float voltage);
void calibrate();

void setup()
{
  // Establish connection to serial monitor
  Serial.begin(9600);

  Wire.begin(23, 22);
  
  //Setup I/O Extender
  io.begin(0x3E);
  for (int i = 0; i < 16; i++) 
  {
    io.pinMode(i, ANALOG_OUTPUT);  
  }
  //Set all pins to 0v
  for(int j = 0; j<16; j++) {
    io.analogWrite(j, 255);
  }
  //Initialize toe-box to an "off"-state
  io.analogWrite(2, 167);
  io.analogWrite(4, 167);
  io.analogWrite(5, 167);
  io.analogWrite(6, 167);
  io.analogWrite(7, 167);
  io.analogWrite(12, 167);
  //Setup accelerometer LIS3DH and calibrate for counting steps
  Serial.println("Initialising Accelerometer");
  delay(1000);
  accel.begin();
  delay(1000);
  Serial.println("Calibrating Stepcounter");
  calibrate();


}

void loop()
{ 
  //Update acceloremeter and do stepcount
  long now = millis();
  if(now - lastChangeAccelTimeOut >= accelTimeOut)
  {
    lastChangeAccelTimeOut = now;
    //Stepcounter
    
    totvect = sqrt(((accel.readFloatAccelX()-xavg)*(accel.readFloatAccelX()-xavg))+((accel.readFloatAccelY()-yavg)*(accel.readFloatAccelY()-yavg))+((accel.readFloatAccelZ()-zavg)*(accel.readFloatAccelZ()-zavg)));
    printCalculatedAccels();
    Serial.print(totvect, 3);
    Serial.print("\t");   
    if (totvect>threshold && flag==0)
    {
      steps=steps+1;
      flag=1;
    }
    else if (totvect > threshold && flag==1)
    {
      //do nothing 
    }
    if (totvect <threshold  && flag==1)
    {
      flag=0;
    }
    Serial.print("steps=");
    Serial.println(steps);
    
    //Pronation sensing
    sensorReading0 = analogRead(piezoSensor0);
    sensorReading1 = analogRead(piezoSensor1);
    Serial.print(sensorReading0);
    Serial.print("   ");
    Serial.println(sensorReading1); 
    if (sensorReading0 > sensorMax0)
    {
      sensorMax0 = sensorReading0;
    }
    if (sensorReading1 > sensorMax1)
    {
      sensorMax1 = sensorReading1;
    }
    if ((sensorReading0 == 0) && (sensorReading1 == 0)) 
    {
      if (sensorMax0 > sensorMax1)
      {
        Serial.println("left");
        flagLeft = true;
        flagRight = false;
      }
      else if (sensorMax0 < sensorMax1)
      {
        Serial.println("right");
        flagLeft = false;
        flagRight = true;
      }
      sensorMax0 = 0;
      sensorMax1 = 0;
    }
  }
  //EC-display
  if (steps == 5)
  {
    setDisplayPower(displayPin0, voltage);
    setDisplayWTPower(side, voltage);
  }
  if (steps == 10)
  {
    setDisplayPower(displayPin0, voltage);
    setDisplayPower(displayPin1, voltage);
    setDisplayWTPower(side, voltage);
  }
  if (steps == 15)
  {
    setDisplayPower(displayPin0, voltage);
    setDisplayPower(displayPin1, voltage);
    setDisplayPower(displayPin2, voltage);
    setDisplayWTPower(side, voltage);
  }
  if (steps == 20)
  {
    setDisplayPower(displayPin0, voltage);
    setDisplayPower(displayPin1, voltage);
    setDisplayPower(displayPin2, voltage);
    setDisplayPower(displayPin3, voltage);
    setDisplayWTPower(side, voltage);
  }
  if (steps == 25)
  {
    setDisplayPower(displayPin0, voltage);
    setDisplayPower(displayPin1, voltage);
    setDisplayPower(displayPin2, voltage);
    setDisplayPower(displayPin3, voltage);
    setDisplayPower(displayPin4, voltage);
    setDisplayWTPower(side, voltage);
  }
  if (steps >= 30)
  {
    side = !side;
    setDisplayWTPower(side, voltage);
  }
  if (steps >= 40)
  {
    io.analogWrite(2, 167);
    io.analogWrite(4, 167);
    io.analogWrite(5, 167);
    io.analogWrite(6, 167);
    io.analogWrite(7, 167);
    io.analogWrite(3, 255);
    side = !side;
    setDisplayWTPower(side, voltage);
    steps=0;
  }
  setDisplayPronationPower(voltage);
}

// Output routine for testing purposes
void printCalculatedAccels()
{ 
  Serial.print(accel.readFloatAccelX(), 3);
  Serial.print("\t");
  Serial.print(accel.readFloatAccelY(), 3);
  Serial.print("\t");
  Serial.print(accel.readFloatAccelZ(), 3);
  Serial.print("\t");
}

//Calibration routine for accelerometer before counting steps
void calibrate()
{
  digitalWrite(13,HIGH);
  float sum=0;
  float sum1=0;
  float sum2=0;
  
  for (int i=0;i<100;i++)
  {
    xval[i]=accel.readFloatAccelX();
    yval[i]=accel.readFloatAccelY();
    zval[i]=accel.readFloatAccelZ();
    sum=xval[i]+sum;
    sum1=yval[i]+sum1;
    sum2=zval[i]+sum2;
    delay(10);
  }
  xavg=sum/100.0;
  yavg=sum1/100.0;
  zavg=sum2/100.0;
  Serial.println(xavg);
  Serial.println(yavg);
  Serial.println(zavg);
  delay(1000);
  digitalWrite(13,LOW);
 
}

void setDisplayPower(int pin, float inputVoltage)
{
  // Convert voltage to 255 range
  int v = 255- ((inputVoltage / 3.5) * 255);
  
  io.analogWrite(pin, 255);
  io.analogWrite(displayPinCE, v);
}

void setDisplayWTPower(bool polarity, float inputVoltage)
{
  // Convert voltage to 255 range
  int v = 255- ((inputVoltage / 3.5) * 255);

  if (polarity)
  {
    io.analogWrite(displayPinWT0, v);
    io.analogWrite(displayPinWT1, 255);
  }
  else
  {
    io.analogWrite(displayPinWT0, 255);
    io.analogWrite(displayPinWT1, v);
  }
}

void setDisplayPronationPower(float inputVoltage)
{
  // Convert voltage to 255 range
  int v = 255- ((inputVoltage / 3.5) * 255);
  
  if (flagLeft)
  {
    io.analogWrite(displayPinPronationIn0, v);
    io.analogWrite(displayPinPronationIn1, 255);
    io.analogWrite(displayPinPronationOut0, 255);
    io.analogWrite(displayPinPronationOut1, v);
  }
  else if (flagRight)
  {
    io.analogWrite(displayPinPronationIn0, 255);
    io.analogWrite(displayPinPronationIn1, v);
    io.analogWrite(displayPinPronationOut0, v);
    io.analogWrite(displayPinPronationOut1, 255);
  }
  else
  {
    io.analogWrite(displayPinPronationIn0, 255);
    io.analogWrite(displayPinPronationIn1, 255);
    io.analogWrite(displayPinPronationOut0, 255);
    io.analogWrite(displayPinPronationOut1, 255);
  }
}
