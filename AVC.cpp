//AVC Challenge
//Team:	BTN

//RPi IP Address: 10.140.30.158
//IMPORT libraries
#include <stdio.h>
#include <time.h>
#include "E101.h"

void sector1(){
   //connects to server with the ip address 130.195.6.196
   connect_to_server("130.195.6.196", 1024);
   //sends a message to the connected server
   send_to_server("Please");
   //receives message from the connected server
   char message[24];
   int count = 0;
   while(message == null && count < 100){
      receive_from_server(message); 
      count++;
   }
   //reply to server
  send_to_server(message);
  sleep1(, 500000);

  return;
}

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
    if(nwp>250){
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
	sector1();
	sector2();
}

