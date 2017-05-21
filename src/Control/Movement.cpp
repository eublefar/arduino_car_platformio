#include "Arduino.h"
#include "Movement.h"
#include "Sensors/Wheels.h"

#define ALPHA 1
int Movement::range = 0;


Movement::Movement(uint8 pinLeft1, uint8 pinLeft2, uint8 pinRight1, uint8 pinRight2)
{

	leftPin1 = pinLeft1;
	leftPin2 = pinLeft2;
	rightPin1 = pinRight1;
	rightPin2 = pinRight2;

	pinMode(pinLeft1,OUTPUT);
	pinMode(pinLeft2,OUTPUT);

	pinMode(pinRight1,OUTPUT);
	pinMode(pinRight2,OUTPUT);


	digitalWrite(pinLeft1, LOW);
	digitalWrite(pinLeft2, LOW);

	digitalWrite(pinRight1, HIGH);
	digitalWrite(pinRight2, HIGH);


	position = new Position();
}

void Movement::updateRPM(uint8 side, double rpm)
{
	if(side == LEFT)
	{
		current_rpm_left = rpm;
	}
	else if(side == RIGHT)
	{
		current_rpm_right = rpm;
	}
}

void Movement::updateOutput()
{
	evalState();
	if(state_changed)
	{
		Serial1.println(state);
		if (state & TURNING_LEFT || state & DRIVING_REVERSE) {
			digitalWrite(leftPin1, LOW);
			mod1 = 0;
		}
		else if(state == STOP)
		{
			mod1 = 255;
			stop(LEFT);
		}
		else if(state & DRIVING || state & TURNING_RIGHT)
		{
			digitalWrite(leftPin1, HIGH);
			mod1 = 255;
		}

		if (state & TURNING_RIGHT || state & DRIVING_REVERSE)
		{
			digitalWrite(rightPin1, HIGH);
			mod2 = 255;
		}
		else if(state == STOP)
		{
			mod2 = 255;
			stop(RIGHT);
		}
		else if (state & DRIVING || state & TURNING_LEFT)
		{
			digitalWrite(rightPin1, LOW);
			mod2 = 0;
		}
		state_changed = false;
	}

	global_speed_left = global_speed_left + ALPHA*(target_velocity_left - ((float)current_rpm_left));
	if(global_speed_left >254)
	{
		global_speed_left = 254;
	}
	else if (global_speed_left <0)
	{
		global_speed_left = 0;
	}
	analogWrite(leftPin2, abs(mod1 -global_speed_left));


	global_speed_right = global_speed_right + ALPHA*(target_velocity_right - ((float)current_rpm_right));
	if(global_speed_right >254)
	{
		global_speed_right = 254;
	}
	else if (global_speed_right <0)
	{
		global_speed_right = 0;
	}
	analogWrite(rightPin2, abs(mod2 -global_speed_right));
}


void Movement::checkStop()
{
	position->updateAngles();
	if(state & MOVING)
	{
			if(Wheels::getCountRight() >=range && Wheels::getCountLeft() >= range)
			{
					stop(LEFT);
					stop(RIGHT);
					range = 0;
					state = STOP;
			}
	}
	else if(state & (TURNING_LEFT|TURNING_RIGHT))
	{
		if((position->getYawAngle()>=target_angle) && (position->getYawAngle()<=target_angle+10))
		{
			Serial.println(String("Turn STOP angle = ") + position->getYawAngle());
			stop(LEFT);
			stop(RIGHT);
			angle = 0;
			target_angle = 0;
			state = STOP;
		}
	}

}

void Movement::evalState()
{
	if(target_velocity_left == 0 && target_velocity_right == 0)
	{
		if(state != STOP)
		{
			state = STOP;
			state_changed = true;
		}
	}
	else if(reverse_left && reverse_right)
	{
		if(state != DRIVING_REVERSE)
		{
			state = DRIVING_REVERSE;
			state_changed = true;
		}
	}
	else if(reverse_left)
	{
		if(state != TURNING_LEFT)
		{
			state = TURNING_LEFT;
			state_changed = true;
		}
	}
	else if(reverse_right)
	{
		if(state != TURNING_RIGHT)
		{
			state = TURNING_RIGHT;
			state_changed = true;
		}
	} else  {
		if(state != DRIVING)
		{
			state = DRIVING;
			state_changed = true;
		}
	}

	if(range && (state & (DRIVING_REVERSE | DRIVING))) {
		 state = (State) ((int)state |(int) MOVING);
	}
}


void Movement::stop(int side)
{
	if(side == LEFT)
	{
		digitalWrite(leftPin1, HIGH);
		digitalWrite(leftPin2, HIGH);
		global_speed_left = 0;
		mod1=255;
	}
	else if(side == RIGHT)
	{
		digitalWrite(rightPin1, HIGH);
		digitalWrite(rightPin2, HIGH);
		global_speed_right = 0;
		mod2=255;
	}
	setRPM(side, 0);
}

void Movement::setRPM(uint8 side,int target)
{

	if(side == LEFT)
	{
		if(target < 0)
		{
			target_velocity_left = -1*target;
			this->reverse_left = true;
			// Serial.println(target_velocity_left);
		}
		else
		{
			target_velocity_left = target;
			this->reverse_left = false;
		}
	}
	if(side == RIGHT)
	{
		if(target < 0)
		{
			target_velocity_right = -1*target;
			this->reverse_right = true;
			// Serial.println(target_velocity_right);
		}
		else
		{
			target_velocity_right = target;
			this->reverse_right = false;
		}
	}
}


int Movement::getGlobalSpeed(int side)
{
	if(side == LEFT)
	{
		return global_speed_left;
	}
	else if(side == RIGHT)
	{
		return global_speed_right;
	}
}

void Movement::move(uint8 range, int speed)
{

	setRPM(RIGHT, speed);
	setRPM(LEFT, speed);

	Movement::range = range;
	Wheels::startCountingRight();
	Wheels::startCountingLeft();
	//Serial.println(timeToDelay);
}


void Movement::turn(uint8 side, uint8 degrees)
{
		// Serial.println(String("TURN BIACH123 ") + side + " " + LEFT);
		angle = position->getYawAngle();
		if(side == LEFT)
		{
			// Serial.println("TURN BIACH");
			setRPM(LEFT, -70);

			setRPM(RIGHT, 70);
		}
		else if(side == RIGHT)
		{
			setRPM(RIGHT, -70);
			setRPM(LEFT, 70);
		}
		evalState();
		target_angle = calculateTargetAngle(angle, degrees);

		Serial.println(String("Turn START angle = ") + position->getYawAngle() + " target_angle = " + target_angle);
}

float Movement::calculateTargetAngle(float angle, float degrees)
{
	if((angle + degrees) >= 360 && (state & TURNING_LEFT))
	{
		return (angle + degrees)- 360;
	}
	else if ((angle - degrees) < 0 && (state & TURNING_RIGHT))
	{
			return angle -degrees + 360;
	}
	else if(state & TURNING_LEFT)
	{
		return angle + degrees;
	}
	 else if(state & TURNING_RIGHT)
	 {
		 return angle - degrees;
	 }
}
