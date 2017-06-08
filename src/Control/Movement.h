#ifndef Movement_h
#define Movement_h

#include "Arduino.h"
#include<DueTimer.h>
#include "Sensors/Position.h"
#include <functional>
#define LEFT 0
#define RIGHT 1

enum State{ DRIVING = B00001,
						DRIVING_REVERSE = B00010,
						STOP = B00000,
						TURNING_LEFT = B00100,
						TURNING_RIGHT = B01000,
						MOVING = B10000
					};

typedef uint8_t uint8;
class Movement
{
	private:

	uint8 leftPin1;
	uint8 leftPin2;
	uint8 rightPin1;
	uint8 rightPin2;
	static int range;
	bool state_changed= false;
	bool reverse_left= false, reverse_right= false;

	int mod1 = 0,mod2 = 0;

	int global_speed_right = 0;
	int global_speed_left = 0;

	int current_rpm_left = 0;
	int current_rpm_right = 0;

	int target_velocity_left = 0;
	int target_velocity_right = 0;

	float angle = 0;
	float target_angle = 0;
	Position *position;

	std::function<void()> callback_on_obstacle;

	void evalState();

public:

	State state;
	Movement(uint8 pinLeft1, uint8 pinLeft2, uint8 pinRight1, uint8 pinRight2);
	float getBearing(){ return position->getYawAngle();}
	void updateRPM(uint8 side, double rpm);
	void updateOutput();
	void setRPM(uint8 side,int target);
	void move(uint8 range, int speed);
	void turn(uint8 side, uint8 degrees);
	void setCallbackOnObstacle(std::function<void()> callback) {callback_on_obstacle = callback;}
	int getGlobalSpeed(int side);
	void stop(int side);
	float calculateTargetAngle(float angle, float degrees);
	void checkStop();

};


#endif
