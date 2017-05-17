#ifndef Position_h
#define Position_h
#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU6050.h"

    #include "Wire.h"

#define INTERRUPT_PIN 40
class Position
{
private:
  bool dmpReady = false;  // set true if DMP init was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];
  MPU6050 mpu;


  // indicates whether MPU interrupt pin has gone high
  static volatile bool mpuInterrupt;

  static void dmpDataReady()
  {
    mpuInterrupt = true;
  }

public:

  Position();

  void updateAngles();

  float getYawAngle();
  float getPitchAngle();
  float getRollAngle();
};

#endif
