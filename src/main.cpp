#ifndef UNIT_TEST123

#include <ISADefinitions.h>
#include <DueTimer.h>
#include <Control/movement.h>
#include <Sensors/Position.h>
#include <Sensors/Wheels.h>


Movement *movement;
/////////////////////////////////////////////////
//Krótki opis klas:
// Wheels - statyczna klasa do wczytywania sensorów halla
// Position - klasa do wczytywania wartości z MPU6050 (gyro + akcelerometer)
// Movement - klasa do wysokopoziomowej kontroli napędami. Używa klasy sensorów i udostępnia pomiary.
//
//@author Magda i Nikita
//
////////////////////////////////////////////////
void setup() {

  Serial1.begin(115200);
  Serial.begin(115200);
  Wheels::init(45, 44);
  movement = new Movement(SERVO1, SERVO2, SERVO3, SERVO4);
  movement->updateOutput();


////////////////// Kalibracja
  float last_angle = 0;
  while(String(last_angle = movement->getBearing())=="ovf"){movement->updateOutput();}
  int cnt =0;
  movement->updateOutput();
  delay(5000);
  while(cnt<60) {
      float angle = 0;
      movement->updateOutput();
      movement->updateOutput();
      while(String(angle = movement->getBearing())=="ovf"){movement->updateOutput();}
      if (abs(last_angle - angle)<=0.001) {
        cnt++;
      }
      Serial.println(String(abs(last_angle - angle)) + "  cnt= " + cnt + " " + last_angle + " " + angle);
      delay(100);
      movement->updateOutput();
      movement->updateOutput();
      while(String(last_angle = movement->getBearing())=="ovf"){movement->updateOutput();}
  }
}


String msg = "";
String msg1 = "";
int cmd = 0;
int mils = 0;


void loop() {
    // int speed = 0;
    // int range = 0;

    movement->updateRPM(LEFT, Wheels::getVelocityLeft());
    movement->updateRPM(RIGHT, Wheels::getVelocityRight());
	  movement->updateOutput();
    // Serial.println(cmd);
    char buf[128];
    char buf_out[16];
    if(cmd == 0 && movement->state==STOP) {
      movement->move(10, 70);
      cmd++;
    } else if(cmd == 1 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 2 && movement->state==STOP) {
      movement->move(10, 70);
      delay(1000);
      cmd++;
    } else if(cmd == 3 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 4 && movement->state==STOP) {
      movement->move(10, 70);
      delay(1000);
      cmd++;
    } else if(cmd == 5 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 6 && movement->state==STOP) {
      movement->move(10, 70);
      delay(1000);
      cmd =0;
    }
  //   sprintf(buf, " vel=%.2f RPM global_speed_right = %d ;vel1=%.2f RPM global_speed_left=%d, reverse_left = %d, reverse_right = %d , state = %d \n",
  //   (float)Wheels::getVelocityLeft() , movement->getGlobalSpeed(RIGHT),
  //   (float)Wheels::getVelocityRight(), movement->getGlobalSpeed(LEFT),
  //   movement->reverse_left,
  //   movement->reverse_right,
  //   movement->state
  // );
  //   Serial.print(buf);
    // msg = Serial.readString();
    // msg.toCharArray(buf_out, 16);
    // msg = strtok(buf_out, " ");
    // Serial.println(msg);
    //  if(msg!="") {
    //    speed = msg.toInt();
    //  }
    //  msg = strtok(NULL, " ");
    // Serial.println(msg);
    //  if(msg!="") {
    //   		range = msg.toInt();
    //       movement->turn(speed, range);
    //  }



}
#endif
