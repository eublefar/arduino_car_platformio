#ifndef Velocity_h
#define Velocity_h

#include "Arduino.h"
#define LEFT 0
#define RIGHT 1

class Wheels
{

  static void interrupt_left();
  static void interrupt_right();
  static void interrupt_no_spin_left();
  static void interrupt_no_spin_right();
  static volatile unsigned long last_measurement_left;
  static volatile unsigned long last_measurement_right;
  static volatile float rpm_left;
  static volatile float rpm_right;
  static volatile int count_left;
  static volatile int count_right;
  static volatile bool counting_left;
  static volatile bool counting_right;
  static int count_callback;
public:
  static void init(uint8_t interruptPinLeft, uint8_t interruptPinRight);
  static float getVelocityLeft();
  static float getVelocityRight();
  static void startCountingRight();
  static int getCountRight();
  static int stopCountingRight();
  static void startCountingLeft();
  static int getCountLeft();
  static int stopCountingLeft();
};



#endif
