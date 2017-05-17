
#ifndef Velocity_cpp
#define Velocity_cpp

#include "Arduino.h"
#include "Wheels.h"
#include "DueTimer.h"

volatile unsigned long Wheels::last_measurement_right = 0;
volatile unsigned long Wheels::last_measurement_left = 0;
#define NO_SPIN_RESET_TIME 700000
volatile float Wheels::rpm_left = 0;
volatile float Wheels::rpm_right = 0;
volatile bool Wheels::counting_left = false;
volatile bool Wheels::counting_right = false;
volatile int  Wheels::count_left = 0;
volatile int  Wheels::count_right = 0;

void Wheels::init(uint8_t interruptPinLeft, uint8_t interruptPinRight)
{
    pinMode(interruptPinLeft,INPUT);
    pinMode(interruptPinRight,INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPinLeft), interrupt_left, CHANGE);
    attachInterrupt(digitalPinToInterrupt(interruptPinRight), interrupt_right, CHANGE);
    Timer7.attachInterrupt(interrupt_no_spin_left);
    Timer7.start(NO_SPIN_RESET_TIME);
    Timer6.attachInterrupt(interrupt_no_spin_right);
    Timer6.start(NO_SPIN_RESET_TIME);
}

void Wheels::interrupt_left()
{
    if(count_left%8 ==0)
    {
      float period = millis() - last_measurement_left;
      last_measurement_left = millis();
      rpm_left = (1.0/period)*1000 * 60; //1 rotation divided by 8 periods of 1/8th rotation = rotation in one milisec.. times 1000 = in rot in one second.. times 60  = RPM
    }
    count_left++;
    Timer7.start(NO_SPIN_RESET_TIME);
}
void Wheels::interrupt_right()
{
    if(count_right%8 ==0)
    {
      float period = millis() - last_measurement_right;
      last_measurement_right = millis();
      rpm_right = (1.0/period)*1000 * 60; //1 rotation divided by 8 periods of 1/8th rotation = rotation in one milisec.. times 1000 = in rot in one second.. times 60  = RPM
    }
    Timer6.start(NO_SPIN_RESET_TIME);
    count_right++;
}

void Wheels::interrupt_no_spin_left()
{
    rpm_left = 0;
}

void Wheels::interrupt_no_spin_right()
{
    rpm_right = 0;
}

float Wheels::getVelocityLeft()
{
    return rpm_left;
}

float Wheels::getVelocityRight()
{
    return rpm_right;
}

void Wheels::startCountingRight()
{
    Wheels::count_right = 0;
    Wheels::counting_right = true;
}

void Wheels::startCountingLeft()
{
    Wheels::count_left = 0;
    Wheels::counting_left = true;
}

int Wheels::getCountRight()
{
    return count_right;
}

int Wheels::getCountLeft()
{
    return count_left;
}

int Wheels::stopCountingRight()
{
    Wheels::counting_right = false;
    return (count_right = 0);
}

int Wheels::stopCountingLeft()
{
    Wheels::counting_left = false;
    return (count_left = 0);
}
#endif
