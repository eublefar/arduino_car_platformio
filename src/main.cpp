#ifdef UNIT_TEST123
#include <functional>
#include <ISADefinitions.h>
#include <DueTimer.h>
#include <Control/movement.h>
#include <Sensors/Position.h>
#include <Sensors/Wheels.h>
#include <Utility/queue.h>
#define TRIG 51
#define ECHO 50


Movement *movement;

bool executing = false;
void checkStopInterrupt()
{
	movement ->checkStop();
}
queue<std::function<void()>> cmd_que;

void addCommand(String command, int a, int b)
{
  Serial1.println(command + " 123 " + (command == "start"));
  if(command == "move")
  {
    cmd_que.push(std::bind(&Movement::move, movement, a, b));
    Serial1.println(String("added move command args = ") + a + " " + b);
  }
  else if(command == "turn")
  {
    cmd_que.push(std::bind(&Movement::turn, movement, a, b));
    Serial1.println(String("added turn command args = ") + a + " " + b);
  }
  else if(command == "setRPM")
  {
    cmd_que.push(std::bind(&Movement::setRPM, movement, a, b));
    Serial1.println(String("added setRPM command args = ") + a + " " + b);
  }
  else if(command == "start")
  {
    executing = true;
    Serial1.println("starting execution");
  }
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
  while(String(last_angle = movement->getBearing())=="ovf"){delay(100);}
  int cnt =0;
  movement->updateOutput();
  Timer5.attachInterrupt(checkStopInterrupt);
  Timer5.start(30000);

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
	pinMode(ECHO, INPUT);
	pinMode(TRIG, OUTPUT);
	movement->setCallbackOnObstacle([]()-> void
	{
		Serial1.println("obstacle");
		movement->stop(0);
		movement->stop(1);
	});

}


String msg = "";
String command = "";
int cmd = 0;
int a = 0;
int b = 0;
char buf_out[16];


void loop() {
    movement->updateRPM(LEFT, Wheels::getVelocityLeft());
    movement->updateRPM(RIGHT, Wheels::getVelocityRight());
	  movement->updateOutput();
    msg = Serial1.readString();

    if(msg!="")
    {
        msg.toCharArray(buf_out, 16);
        msg = strtok(buf_out, " ");
        if(msg!="")
        {
          command = msg;
        }

        msg = strtok(NULL, " ");
        if(msg!="")
        {
          a = msg.toInt();
        } else
        {
          a=0;
        }

				msg = strtok(NULL, " ");
        if(msg!="")
        {
      		b = msg.toInt();
        }
        else
        {
          b=0;
        }

        addCommand(command, a, b);
    }

    if(!cmd_que.empty())
    {

      if(movement->state == STOP && executing)
      {
				Serial1.println("executing");
        cmd_que.front()();
        cmd_que.pop();
      }
    }
    else
    {
      executing = false;
    }

		digitalWrite(TRIG, LOW);
		delayMicroseconds(2);
		// Sets the trigPin on HIGH state for 10 micro seconds
		digitalWrite(TRIG, HIGH);
		delayMicroseconds(10);
		digitalWrite(TRIG, LOW);
		// Reads the echoPin, returns the sound wave travel time in microseconds
		float dist = pulseIn(ECHO, HIGH)*0.034/2;

		if(dist <= 20) {
		//	callback_on_obstacle();
			movement->stop(LEFT);
			movement->stop(RIGHT);
		}
}


#endif
