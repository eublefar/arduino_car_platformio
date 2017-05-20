#ifndef UNIT_TEST123

#include <ISADefinitions.h>
#include <DueTimer.h>
#include <Control/movement.h>
#include <Sensors/Position.h>
#include <Sensors/Wheels.h>





Movement *movement;

void checkStopInterrupt() {
	movement ->checkStop();

}
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
  Timer5.attachInterrupt(checkStopInterrupt);
  Timer5.start(15000);
  while(cnt<60) {
      float angle = 0;
      while(String(angle = movement->getBearing())=="ovf"){delay(100);}
      if (abs(last_angle - angle)<=0.001) {
        cnt++;
      }
      Serial1.println(String(abs(last_angle - angle)) + "  cnt= " + cnt + " " + last_angle + " " + angle);
      delay(100);
      while(String(last_angle = movement->getBearing())=="ovf"){delay(100);}
  }

}


String msg = "";
String msg1 = "";
int cmd = 0;
int a = 0;
int b = 0;
char buf_out[16];


void loop() {
    movement->updateRPM(LEFT, Wheels::getVelocityLeft());
    movement->updateRPM(RIGHT, Wheels::getVelocityRight());
	  movement->updateOutput();
    msg = Serial1.readString();
    if(msg!="") {
        msg.toCharArray(buf_out, 16);
        msg = strtok(buf_out, " ");
        // Serial1.println(msg);
        if(msg!="") {
          a = msg.toInt();
        }
        msg = strtok(NULL, " ");
        // Serial1.println(msg);
        if(msg!="") {
      		b = msg.toInt();
        }
    }
    if(cmd == 0 && movement->state==STOP && a!=0 && b!=0) {
      movement->move(a, 70);
      cmd++;
    } else if(cmd == 1 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 2 && movement->state==STOP) {
      movement->move(b, 70);
      delay(1000);
      cmd++;
    } else if(cmd == 3 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 4 && movement->state==STOP) {
      movement->move(a, 70);
      delay(1000);
      cmd++;
    } else if(cmd == 5 && movement->state==STOP) {
      movement->turn(0,90);
      cmd++;
    } else if(cmd == 6 && movement->state==STOP) {
      movement->move(b, 70);
      a=0;
      b=0;
      cmd=0;
    }
  //   sprintf(buf, " vel=%.2f RPM global_speed_right = %d ;vel1=%.2f RPM global_speed_left=%d, reverse_left = %d, reverse_right = %d , state = %d \n",
  //   (float)Wheels::getVelocityLeft() , movement->getGlobalSpeed(RIGHT),
  //   (float)Wheels::getVelocityRight(), movement->getGlobalSpeed(LEFT),
  //   movement->reverse_left,
  //   movement->reverse_right,
  //   movement->state
  // );
  //   Serial.print(buf);
}
#endif
