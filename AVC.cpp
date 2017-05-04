#include <stdio.h>
#include <time.h>
#include "E101.h"

int main()
{
	init();
	
}

//module to drive robot straight at variable speeds
void drive_straight(int speed){
	set_motor(1, speed);
	set_motor(2, -speed);
	return;
}

//module to make the robot stop
void stop(){
	set_motor(1, 0);
	set_motor(2, 0);
	return;
}

//module to turn through ** degrees
void turn(double degree, bool bool_right){
	int left;
	int right;
	if (bool_right){
		left = 100;
		right = -150;		
	} else if (!bool_right){
		left = -150;
		right = 100;
	} else {
		return;
	}
		
	int turn_time = degree/360 * //time for full circle
	set_motor(1, 100); //left
	set_motor(2, -150); //right
	sleep1(0, turn_time); //alter to turn through a certain angle
	set_motor(1, 0);
	set_motor(2, 0);
	return;
}

