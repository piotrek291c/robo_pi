#include <iostream>
#include <wiringPi.h>
#include "libSonar.h"

Sonar::Sonar(){}

void Sonar::init(int trigger, int echo)
{
    this->trigger=trigger;
    this->echo=echo;
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);
    delay(350);
}

double Sonar::distance(int timeout)
{
    delay(32);

    digitalWrite(trigger, HIGH);
    delayMicroseconds(11);
    digitalWrite(trigger, LOW);

    now=micros();

    while ((digitalRead(echo) == LOW ) && (signed(micros()-now)<timeout));
        recordPulseLength();

    travelTimeUsec = endTimeUsec - startTimeUsec;
    distanceMeters = 100*((travelTimeUsec/1000000.0)*340.29)/2;

    return distanceMeters;
}

void Sonar::recordPulseLength()
{
    startTimeUsec = micros();
    while ( digitalRead(echo) == HIGH );
    endTimeUsec = micros();
}
