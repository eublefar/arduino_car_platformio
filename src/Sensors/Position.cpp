// License of free will.
// Do whatever you like restricted only by the license of this code's depencies
#ifndef Position_cpp
#define Position_cpp

#include "Arduino.h"
#include "Position.h"
//#include "MPU6050_6Axis_MotionApps20.h"

volatile bool Position::mpuInterrupt = false;


Position::Position()
{
    Wire.begin();
    Wire.setClock(200000);
    Serial.begin(115200);
    while (!Serial);
    Serial.println("HI!");
    mpu.initialize();

    Serial.println(mpu.testConnection() ?
    F("MPU6050 connection successful") :
    F("MPU6050 connection failed"));
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1688);
    if (devStatus == 0)
    {
      // turn on the DMP, now that it's ready
      Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
    }
     else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void Position::updateAngles()
{
  while(!mpuInterrupt);

  {
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      // Serial.println(F("FIFO overflow!"));
      // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      //Serial.println(this->getYawAngle());
    }
  }
}


float Position::getYawAngle()
{
  float angle = ypr[0] * 180/M_PI;
  if(angle<0) {
    angle = 180 + (180 + angle);
  }
  return angle;
}

float Position::getPitchAngle()
{
  float angle = ypr[1] * 180/M_PI;
  if(angle<0) {
    angle = 180 + (180 + angle);
  }
  return angle;
}

float Position::getRollAngle()
{
  float angle = ypr[2] * 180/M_PI;
  if(angle<0) {
    angle = 180 + (180 + angle);
  }
  return angle;
}


#endif
