#include "Arduino.h"
#include <Wire.h>


class Ultrasonic
{
    public:
        Ultrasonic(int pin);
        void DistanceMeasure(void);
        long microsecondsToCentimeters(void);
        long microsecondsToInches(void);
        long getDuration(void){return duration;};
        int getPin(void){return _pin;};
    private:
        int _pin;//pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
        long duration;// the Pulse time received;
};

Ultrasonic::Ultrasonic(int pin)
{
    _pin = pin;
}
/*Begin the detection and get the pulse back signal*/
void Ultrasonic::DistanceMeasure(void)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_pin,LOW);
    pinMode(_pin,INPUT);
    duration = pulseIn(_pin,HIGH);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void)
{
    return duration/29/2; 
}

/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void)
{
    return duration/74/2; 
}

//initialize sensor  
Ultrasonic ultrasonic1(7);
Ultrasonic ultrasonic2(8);

bool entryOn = false;
void setup() {
  // put your setup code here, to run once:    
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(2, OUTPUT);
    Wire.begin(8);
    Wire.onRequest(requestEvent); // register event
}

bool prevhit1 = false;
bool prevhit2 = false;

// an 
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("reading xy");
  //  flash(1);
     long RangeInInches1;
     long RangeInInches2;

     ultrasonic2.DistanceMeasure();// get the current signal time;
     RangeInInches2 = ultrasonic2.microsecondsToInches();//convert the time to inches;

     ultrasonic1.DistanceMeasure();// get the current signal time;
     RangeInInches1 = ultrasonic1.microsecondsToInches();//convert the time to inches;

//Serial.println(RangeInInches1);
//Serial.println(RangeInInches2);


    bool hit1 = false;
    bool hit2 = false;
    long set = 30;
/*
Serial.println(hit1);
Serial.println(hit2);
*/
    if(RangeInInches1 < set) hit1 = true;
    if(RangeInInches2 < set) hit2 = true;

  if(hit1 == prevhit1 && hit2 == prevhit2){
    // nothing's changed, don't do anything
    delay(100);
    return;
  }
    
//Serial.println("2");

  if(hit1){
    Serial.println("hit1");
  }else{
  //  Serial.println("no hit1");
  }
  if(hit2){
    Serial.println("hit2");
  }else{
//    Serial.println("no hit2");
  }


  if(hit1 && prevhit1 && hit2 && !prevhit2){
    Serial.println("entry?");
    entryOn = true;
    
  }

  prevhit1 = hit1;
  prevhit2 = hit2;
 delay(100); 
}


void requestEvent() {
  if(entryOn == true){
    Wire.write("entry"); // respond with message of 5 bytes
    // as expected by master
    entryOn = false;
  }
}



void flash(int num){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  for(int i = 0; i < num; i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }
  
}

