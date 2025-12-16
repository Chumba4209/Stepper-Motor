/*
  Stepper Motor Test
  with 9-42V DC Microstep driver
  
*/
#include <Arduino.h>
// Defin pins
int reverseSwitch = 8;  // Push button for reverse
int driverPUL = 7;    // PUL- pin
int driverDIR = 6;    // DIR- pin
int spd = A0;     // Potentiometer

// Variables

int pd = 500;       // Pulse Delay period
boolean setdir = LOW; // Set Direction

// Interrupt Handler

void revmotor (){

  setdir = !setdir;
  
}


void setup() {

  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(reverseSwitch), revmotor, FALLING);
  
}

void loop() {
  
    pd = map((analogRead(spd)),0,1023,2000,50);
    digitalWrite(driverDIR,setdir);
    digitalWrite(driverPUL,HIGH);
    delayMicroseconds(pd);
    digitalWrite(driverPUL,LOW);
    delayMicroseconds(pd);
 
}