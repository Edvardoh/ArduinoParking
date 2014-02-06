// New-Bright RC Car Control
// Input from 2 MaxSonar Ultrasonic Range finders for positioning and collision avoidance
// Output signal controls 2 VEX motor controllers which control the steering and drive motors on the RC car
// Pushbutton toggles signal to VEX motor controllers

#include <Servo.h> 

// create servo objects
Servo Drive;  
Servo Steering; 

//assign devices to I/O pins on Arduino
const int pwPinR = 9;
const int pwPinL = 8;
const int buttonPin = 13;

//variables to store values
long pulseR, inchesR, cmR, pulseL, inchesL, cmL, sumR, avgR, sumL, avgL;

int buttonState = 0;
int count1 = 0;
int buttonToggle = 0;
int lastButtonState = 0;

void setup() 
{ 
  //assign inputs/outputs
  pinMode(pwPinR, INPUT);
  pinMode(pwPinL, INPUT);
  pinMode(buttonPin, INPUT);

  //attach motors to I/O pins  
  Drive.attach(9);
  Steering.attach(7);
  //Open up a serial connection to shoot the results back to the PC console for debugging
  Serial.begin(9600);
} 
 
void loop() 
{ 
//check whether button has been pressed odd number of times. If odd, stop car. If even, proceed.
buttonState = digitalRead(buttonPin);
if (buttonState != lastButtonState) {
    if (buttonState == HIGH){
      count1++;
      buttonToggle =  count1 % 2;
    }
    else {}
}
else{}
lastButtonState = buttonState;

if  (buttonToggle != 0){
  Serial.println("car stopped");
  Drive.write(0);
  Steering.write(90);
}
  else{
  Serial.println("car running");   

//Ultrasonic Sensor Pulse/Reading. Averaging is used to smooth sensor readings.
//Right sensor
avgR = 0;
sumR = 0;
for (int i = 0; i <= 1; i++)
 {  
    pulseR = pulseIn(pwPinR, HIGH);
  //speed of sound is 147uS per inch
   inchesR = pulseR/147;
  //change inches to centimetres
  cmR = inchesR * 2.54;
  sumR = sumR + cmR;
  /*
  Serial.print(cmR);
  Serial.print("Right cm");
  Serial.println();
  //delay(200);
  */
 }
avgR = sumR/2;
Serial.print(avgR);
Serial.print(" avg cm Right");
Serial.println();
//Left sensor
sumL = 0;
avgL = 0;
for (int i = 0; i <= 1; i++)
 {
    pulseL = pulseIn(pwPinL, HIGH);
  //147uS per inch
  inchesL = pulseL/147;
  //change inches to centimetres
  cmL = inchesL * 2.54;
  sumL = sumL + cmL;
  /*
  Serial.print(cmL);
  Serial.print("Left cm");
  Serial.println();
  //delay(200); 
  */
 }
avgL = sumL/2;
Serial.print(avgL);
Serial.print(" avg cm Left");
Serial.println();
 
  //Drive and emergency stop
  if ((avgR + avgL)/2 > 50)
    {
      //Check allignment
      if (avgR > 1.05*avgL)
        {
        Serial.println("turning left");
        Drive.write(107);
        Steering.write(120);
        delay(300);
        Steering.write(90);
        } 
      else if (avgR < .95*avgL)
        {
        Serial.println("turning right");
        Drive.write(107);
        Steering.write(60);
        delay(300);
        Steering.write(90);
        } 
       else
        {
         Drive.write(107);
         Steering.write(90);
         delay(100);
        }     
    }
    
    else
    {
    //Stop if object is closer than 50 cm
    Steering.write(90);
    Drive.write(90); 
    delay(300);
    }
    
} 
}
