#ifdef UNIT_TEST

#include <ISADefinitions.h>
#include <DueTimer.h>

#include <Position.h>


#define UPDATE_FREQ 500



float angle = 0;
Position *pos;
void setup() {



      Serial.begin(115200);
      delay(1000);
      pos = new Position();

      pinMode(10,OUTPUT);
      pinMode(11,OUTPUT);

      pinMode(12,OUTPUT);
      pinMode(13,OUTPUT);

      digitalWrite(10,LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      delay(10000);
      angle = pos->getYawAngle();

}



void loop() {
  if(abs(pos->getYawAngle()- angle) <=90) {
      pos->updateAngles();
      Serial.println(String(pos->getYawAngle())+ " " + String(pos->getPitchAngle()) );
  }
  else
  {
    digitalWrite(11, LOW);
      digitalWrite(13, LOW);
      Serial.println("got it");
      while(true);
  }
      // Serial.print("hi");
}
#endif
