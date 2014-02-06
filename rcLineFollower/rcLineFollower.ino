// New-Bright RC Car Control
// Input from 2 infrared light sensors for line following
// Input from 1 MaxSonar Ultrasonic Range finder for collision avoidance
// Output signal controls 2 VEX motor controllers which control the steering and drive motors on the RC car
// Pushbutton toggles signal to VEX motor controllers

#include <Servo.h> 

Servo Drive;  // create servo objects
Servo Steering;

//assign devices to I/O pins on Arduino
const int RLPin = 10;
const int LLPin = 11;
const int pwPin = 3;
const int buttonPin = 13;

//variables to store values
long Rstate, Lstate, pulse, cm, avgcm, sumcm;
int buttonState = 0;
int count1 = 0;
int buttonToggle = 0;
int lastButtonState = 0;

void setup() 
{ 
  //assign inputs/outputs
  pinMode(RLPin, INPUT);
  pinMode(LLPin, INPUT);
  pinMode(pwPin, INPUT);
  pinMode(buttonPin, INPUT);
  
  //attach motors to I/O pins
  Drive.attach(7);
  Steering.attach(6);
  
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
  
//Ultrasonic Sensor Pulse/Reading
 avgcm = 0;
 sumcm = 0;
 for (int i = 0; i <= 1; i++)
 {
 pulse = pulseIn(pwPin, HIGH);
  //speed of sound is 147uS per inch
   cm = (pulse/24.75 - 1.33)*2.54;
  //change inches to centimetres
  sumcm = sumcm + cm;
  //Serial.print(cm);
  //Serial.print(" cm");
  //Serial.println();
 }
 avgcm = sumcm/2;
 Serial.print(avgcm);
 Serial.print(" avg cm");
 Serial.println();
 
//initialize line detectors
 Rstate = digitalRead(RLPin);
 Lstate = digitalRead(LLPin);
 Serial.print(Rstate);
 Serial.print("Right");
 Serial.println();
 Serial.print(Lstate);
 Serial.print("Left");
 Serial.println();

if (avgcm > 65)
{
  //Line Following program
  if (Rstate == 0)
  {
     if (Lstate == 0)
      {
        Serial.println("forward");
        Drive.write(107);
       delay(100);
      } 
     else
      {
        Serial.println("forward left");
       Steering.write(120);
       Drive.write(107);
       delay(300);
       Steering.write(90); 
      }
   }
  else
  {
     if (Lstate == 0)
      {
        Serial.println("forward right");
        Steering.write(60);
       Drive.write(107);
       delay(300);
       Steering.write(90);
      } 
     else
      {
         Serial.println("forward");
       Steering.write(90);
       Drive.write(107);
      delay(100); 
      }
  }
}
else
{
//stop if closest object is < 50cm
Steering.write(0);
Drive.write(90);
delay(300);
}
}
}
