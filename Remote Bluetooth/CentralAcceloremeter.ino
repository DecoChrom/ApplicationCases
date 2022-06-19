//y & z for balance y = + means leaNING Right, z = + means leanin back

//Analog read pins
const int xPin = 0;
const int yPin = 1;
const int zPin = 2;

int xRead,yRead,zRead;

//to hold the caculated values
double x;
double y;
double z;

float xRaw,yRaw,zRaw;  
float xAccel,yAccel,zAccel, yAccelPrevious, zAccelPrevious;

int xRawMin = 624;
int xRawMax = 396;
 
int yRawMin = 448;
int yRawMax = 566;
 
int zRawMin = 447;
int zRawMax = 581;

int waitTime = 50;
float treshold = 0.12;


// angle estimated from kalman
float angleXAcc;
float angleYAcc;

boolean processing = false;
boolean printAngles = false;

void setup(){
  Serial.begin(115200);
  Serial.println("Occhio");
  pinMode(zPin,INPUT); 
  //printCalVals();
}
  
void display2(){
  if (abs(yAccel)>treshold || abs(zAccel)>treshold){
    Serial.write('1');
    //Serial.println("Movement");
  }
  else if (abs(yAccel)<treshold && abs(zAccel)<treshold){
    Serial.write('0');
    //Serial.println("Still");
  }
  delay(waitTime);
}

void display1(){  
  if (-1.1<=xAccel && xAccel<=-0.9 && -0.1<=yAccel && yAccel<=0.1 && -0.1<=zAccel && zAccel<=0.1){
    Serial.write('0');
    //Serial.println('X');
  }
  if (-0.1<=xAccel && xAccel<=0.1 && -01.1<=yAccel && yAccel<=-0.9 && -0.1<=zAccel && zAccel<=0.1){
    Serial.write('1');
    //Serial.println('Y');
  }
  if (-0.1<=xAccel && xAccel<=0.1 && -0.1<=yAccel && yAccel<=0.1 && -01.1<=zAccel && zAccel<=-0.9){
    Serial.write('2');
    //Serial.println('Z');
  }
  delay(100);
}

void scaleAccs(){
  float xScaled = map(xRead, xRawMin, xRawMax, -1000, 1000);
  float yScaled = map(yRead, yRawMin, yRawMax, -1000, 1000);
  float zScaled = map(zRead, zRawMin, zRawMax, -1000, 1000);
  
  // re-scale to fractional Gs
  xAccel = xScaled / 1000.0;
  yAccel = yScaled / 1000.0;
  zAccel = zScaled / 1000.0; 
}

void readAcc(){
  xRead = analogRead(xPin);
  yRead = analogRead(yPin);
  zRead = analogRead(zPin); 
}

void printVals(){
  Serial.print("xG: ");
  Serial.print(xAccel);
  Serial.print(" | yG: ");
  Serial.print(yAccel);
  Serial.print(" | zG: ");
  Serial.print(zAccel);
  Serial.print("      X: ");
  Serial.print(xRead);
  Serial.print(" | Y: ");
  Serial.print(yRead);
  Serial.print(" | Z: ");
  Serial.print(zRead);
  Serial.println();
}

void printCalVals(){
  Serial.print("X _ min max: ");
  Serial.print(xRawMin);
  Serial.print(" : ");
  Serial.println(xRawMax);
  Serial.print("Y _ min max: ");
  Serial.print(yRawMin);
  Serial.print(" : ");
  Serial.println(yRawMax);
  Serial.print("Z _ min max: ");
  Serial.print(zRawMin);
  Serial.print(" : ");
  Serial.println(zRawMax);  
}


void serialRoutine(){
 if (Serial.available())
 {
   char t = Serial.read();
   Serial.println(t);
   if (t == 'x')
   {   
     char sec = Serial.read();
     if (sec == 'm')
     {
      xRawMin = xRead;
       Serial.print("Min X: ");
       Serial.println(xRawMin);
     }
     else if (sec == 'M')
     {
       xRawMax = xRead;
       Serial.print("Max X: ");
       Serial.println(xRawMax);
     }
   }
   else if (t == 'y')
   {   
     char sec = Serial.read();
     if (sec == 'm')
     {
      yRawMin = yRead;
       Serial.print("Min Y: ");
       Serial.println(yRawMin);
     }
     else if (sec == 'M')
     {
       yRawMax = yRead;
       Serial.print("Max Y: ");
       Serial.println(yRawMax);
     }
   }
   else if (t == 'z')
   {   
     char sec = Serial.read();
     if (sec == 'm')
     {
      zRawMin = zRead;
       Serial.print("Min Z: ");
       Serial.println(zRawMin);
     }
     else if (sec == 'M')
     {
       zRawMax = zRead;
       Serial.print("Max Z: ");
       Serial.println(zRawMax);
     }
   }
   else if (t=='v')
   {
     Serial.print("Min x: ");
     Serial.println(xRawMin);
     Serial.print("Max x: ");
     Serial.println(xRawMax);
     Serial.print("Min Y: ");
     Serial.println(yRawMin);
     Serial.print("Max Y: ");
     Serial.println(yRawMax);
     Serial.print("Min Z: ");
     Serial.println(zRawMin);
     Serial.print("Max Z: ");
     Serial.println(zRawMax);
   }
 } 
}

void loop(){
  //read the analog values from the accelerometer
  readAcc();
  serialRoutine();
  scaleAccs();
  //printVals();
//  display1();
  display2();
}
