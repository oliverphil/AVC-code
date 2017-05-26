//AVC Challenge
//Team:	BTN

//RPi IP Address: 10.140.30.158
//IMPORT libraries

//sensors: 0-left 1-right 2-front
#include <stdio.h>
#include <time.h>
#include "E101.h"

int count = 0;

void sector1(){
   //connects to server with the ip address 130.195.6.196
   char ip[15] = {'1','3', '0', '.', '1', '9', '5', '.', '6', '.', '1', '9', '6'};
   connect_to_server(ip, 1024);
   //sends a message to the connected server
   char ask[24] = {'P', 'l', 'e', 'a', 's', 'e', 'h', 'e'};
    send_to_server(ask);
   //receives message from the connected server
   char message[24];
   receive_from_server(message);
   //reply to server
  send_to_server(message);
  sleep1(0, 500000);
  return;
}

void back(){
  set_motor(1, -40);
  set_motor(2, 130);
}

void sector2(){
  double previous_error = 1;
  double derivative_signal;
  while(true){
    double current_error = 0;
    double kp = 0.5;
    double kd = 0.8;
    double proportional_signal;
    int nwp = 0;
    take_picture();
    if(get_pixel(120, 160, 0)>200 && get_pixel(120, 160, 1)<80 && get_pixel(120, 160, 2)<80){
	    for (int i=0; i<320; i++){
		double pixel = get_pixel(120, i, 3);
		if(pixel>127){
		  nwp++;
		  pixel = 1;
		} else {
		  pixel = 0;
		}
		int error = (i-160)*pixel;
		current_error += error;
	    }
	    current_error /= nwp;
	    proportional_signal = (current_error)*kp;
	    derivative_signal = (current_error-previous_error)*kd;
	    previous_error = current_error;
	    double speed = (proportional_signal+derivative_signal);
	    if(nwp>200){
		speed = -50;
		set_motor(1, speed);
		set_motor(2, speed);
	    } else if(nwp==0){
		   back();
		   sleep1(0, 1500);
	    } else {
		speed = (proportional_signal+derivative_signal);
		if(speed>150){
		   speed = 150;
		}else if(speed<-150){
		   speed = -150;
		}
	    if(current_error > 0){
		set_motor(1, (int)75-speed);
		set_motor(2, (int)-75);
	    } else {
		set_motor(1, 75);
		set_motor(2, -75-speed);
	    }
	    }
		sleep1(0, 6000);
	  } else {
	    return;
    }
  }
}

void sector4(){
    double current_error = 0;
    double kp = 0.5;
    double kd = 0.8;
    double proportional_signal;
    current_error = adc_reading(0)-adc_reading(1); 
    proportional_signal = current_error*kp;
    derivative_signal = (current_error-previous_error)*kd;
    previous_error = current_error;
    double speed = (proportional_signal+derivative_signal);
    if(adc_reading(2)<500){
	if(current_error > 0){
	    set_motor(1, (int)50-speed);
	    set_motor(2, -50);
	} else {
	    set_motor(1, 50);
	    set_motor(2, -50-speed);
	}
    sleep1(0, 6000);
    }else if(count<10){
	set_motor(1, 0);
	set_motor(2, 0);
	sleep1(1, 0);
    }else{
	if(speed > 0){
	    set_motor(1, 50-speed);
	    set_motor(2, 0);
	else{
	    set_motor(1, 0);
	    set_motor(2, -50-speed);
	}
	count = 0;
	sleep1(0, 6000);
    }
}

//OUTPUT. SET wheel speed to zero
void kill(){
	set_motor(2, 0);
	set_motor(1, 0);
}

//RUN main method
int main(){
	init(); //INIT rasberry pi components 
	sector1();
	sector2();
}


