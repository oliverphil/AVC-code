//AVC Challenge
//Team:	BTN

//Quadrant Name:	Curved Line & Straight Line
//Quadrant Number:	2 & 3 of 4

//RPi IP Address: 10.140.30.158
//IMPORT libraries
#include <stdio.h>
#include <time.h>
#include "E101.h"

void back(){
  set_motor(1, -20);
  set_motor(2, 15);
}

void sector2(){
  double previous_error = 1;
  int derivative_signal;
  while(true){
    double current_error = 0;
    int kp = 0.5;
    int kd = 0.2;
    int proportional_signal;    
    int nwp = 0;
    for (int i=0; i<320; i++){
        int pixel = get_pixel(i, 120, 3);
        if(pixel>127){
          nwp++;
          pixel = 1;
        } else {
          pixel = 0;
        }
        int error = (i-160)*pixel;        
        current_error += error;
    }
    proportional_signal = (current_error/nwp)*kp;
    derivative_signal = (current_error-previous_error)*kd;
    previous_error = current_error;
    int speed = proportional_signal+derivative_signal;
    if(nwp>200){
        speed = -50;
        set_motor(1, speed);
        set_motor(2, -speed);
    } else if(nwp<10){
	back();
    } else {
    set_motor(1, 50+speed);
    set_motor(2, -50+speed);
    }
  }
}

//OUTPUT. SET wheel speed to zero
void kill(){
	set_motor(2, 0);
	set_motor(1, 0);
}

//RUN main method
int main(){
	init(); //INIT rasberry pi components //NOTE: should we implement a test method to ensure each component is operating correctly?
	sector2();
}
