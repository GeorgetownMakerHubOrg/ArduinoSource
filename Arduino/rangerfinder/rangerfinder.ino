#include <ArduinoSTL.h>

/***************************************************************************/  
//  Function: Measure the distance to obstacles in front and print the distance
//        value to the serial terminal.The measured distance is from 
//        the range 0 to 400cm(157 inches).
//  Hardware: Ultrasonic Range sensor
//  Arduino IDE: Arduino-1.0
//  Author:  LG   
//  Date:    Jan 17,2013
//  Version: v1.0 modified by FrankieChu
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/*****************************************************************************/
#include "Arduino.h"
#include <vector>

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

//initialize default distance 
long set = 30;

void setup()
{
    Serial.begin(9600);
}

std::vector<String> determine;

void loop()
{   
     long RangeInInches1;
     long RangeInInches2;

     ultrasonic2.DistanceMeasure();// get the current signal time;
     RangeInInches2 = ultrasonic2.microsecondsToInches();//convert the time to inches;


     ultrasonic1.DistanceMeasure();// get the current signal time;
     RangeInInches1 = ultrasonic1.microsecondsToInches();//convert the time to inches;

    bool hit1 = false;
    bool hit2 = false;

    if(RangeInInches1 < set) hit1 = true;
    if(RangeInInches2 < set) hit2 = true;
    
    String coming = " "; 
//    Serial.println(String(RangeInInches1) + " " + String(RangeInInches2));
    if(!hit1 && !hit2)        coming = " ";
    else if(hit1 && !hit2) {
      if(determine.back() != "in") 
          determine.push_back("in");  
    }
    else if(hit1 && hit2) {
      if(determine.back() != "both") 
          determine.push_back("both");  
    }
    else if(!hit1 && hit2) {
      if(determine.back() != "out") 
          determine.push_back("out");  
    }

    if(determine.size() == 3) {
      if(determine.at(0) == "in" && determine.at(1) == "both" && determine.at(2) == "out")
        Serial.println("visitor is coming");
      else if (determine.at(0) == "out" && determine.at(1) == "both" && determine.at(2) == "in")
        Serial.println("visitor is leaving");
//      else Serial.println(determine.at(0) + " " + determine.at(1) + " " +determine.at(2));
      
      determine.clear();
    }
    else if(determine.at(0) == "waiting") 
      determine.clear();
//    else if(determine.at(2) == "waiting") 
//      determine.clear();
//    Serial.println("Visitor is : " + coming);

    delay(100);
}

