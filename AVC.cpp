//AVC Challenge
//Team: BTN

//RPi
//IP Address:	10.140.30.158
//Wheels:	1-left wheel, 2-right wheel //NOTE: is this correct?
//Sensors: 	0-left, 1-right, 2-front

//PID: kp-proportional, kd-derivative //note: ki-integral not implemented

//IMPORT libraries
#include <stdio.h>
#include <time.h>
#include "E101.h"

int count = 0; //NOTE: should this be moved to just before sector 4 for clarity?

//Sector 1 of 4: Gate
void sector1()
{
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
  
  sleep1(0, 500000); //delay the program to allow the gate to open
  return;
}

void back()
{
  set_motor(1, -40); //left wheel reverses slow
  set_motor(2, 130); //right wheel reverses fast //NOTE: motor is inverted, hence the positive value
}

//Sector 2 of 4: Curved line and Sector 3 of 4: Sharp line //NOTE: perhaps rename this method to reflect this?
void sector2()
{
	double previous_error = 1;
	double derivative_signal;

	while(true)
	{
		double current_error = 0; //0=ideal value
		double kp = 0.5;
		double kd = 0.8;
		double proportional_signal;
		int nwp = 0; //number of white pixels

		take_picture(); //take a picture using the camera component connected to the RPi //picture: height=120, width=320

		if(get_pixel(120, 160, 0)>200 && get_pixel(120, 160, 1)<80  && get_pixel(120, 160, 2)<80) //get_pixel(height, width, color type) //color type: 0=red, 1=green, 2=blue, 3=luminosity
		{
			for(int i=0; i<320; i++) //320=image width
			{
				double pixel = get_pixel(120, i, 3); //get_pixel(height, width, color type) //color type: 3=luminosity
			
				if(pixel>127) //'127' represents the luminosity split point, '127' is halfway between black=0 and white=255
				{
				  nwp++;
				  pixel = 1; //pixel is snapped to 'white'
				}
				else
				{
				  pixel = 0; //pixel is snapped to 'black'
				}
				int error = (i-160)*pixel; //(pixel minus half the width of the picture) * pixel=1,0
				current_error += error; //'current_error' is a total sum of the error for each pixel along the width of the picture //current_error = 320 'error' values
			}
			current_error 		= current error/nwp;
			proportional_signal	= (current_error)*kp;
			derivative_signal 	= (current_error-previous_error)*kd;
			previous_error		= current_error;
			
			double speed = (proportional_signal+derivative_signal); //speed = PID output //speed is passed to the RPi motors
			
			if(nwp>200) //if the processed width of the picture was mostly white (at least 200 of the 320 pixels were 'white')
			{
			speed = 50;
			set_motor(1, -speed);  //slot 1=left wheel
			set_motor(2, speed); //slot 2=right wheel
			}
			else if(nwp==0) //else if it was completely black (0 of the 320 pixels were 'white')
			{
			   back();
			   sleep1(0, 1500); //delays the program to allow the robot to reverse longer given the current error, before processing the next error (else the robot will gitter back and forth on the spot)
			}
			else
			{
				speed = (proportional_signal+derivative_signal);
				
				//cap the speed of the motors
				if(speed>150) //'150' = top speed forward
				{
					speed = 150;
				}
				else if(speed<-150) //'-150' = top speed reversing
				{
					speed = -150;
				}
				
				//turn the robot left or right
				if(current_error > 0) //if error is positive: turn right
				{
					set_motor(1, (int)75-speed); //can this just be: 75-(int)speed
					set_motor(2, (int)-75);		 // can this just be: -75
				}
				else //else (if error is 0 or negative): turn left
				{
					set_motor(1, 75);
					set_motor(2, -75-speed);
				}
			}
		sleep1(0, 6000);
		}
		else
		{
			return;
		}
	}
}

//Sector 4 of 4: Maze
void sector4()
{
    double current_error = 0;
    double kp = 0.1;
    double kd = 0.8;
    double proportional_signal;
    double derivative_signal;   
    double previous_error = 0;
	
	while(true)
	{
	    current_error 	= (read_analog(1)-20)-read_analog(0); //read from the IR sensor components connected to the RPi //read_analog(0-left, 1-right, 2-front)
	    proportional_signal = current_error*kp;
	    derivative_signal 	= (current_error-previous_error)*kd;
	    previous_error 	= current_error;
	    double speed 	= (proportional_signal+derivative_signal);
	    
	    if(read_analog(2)<500 || current_error > 50) //if the robot does not detect a wall is directly in front of it //NOTE: clarify this if block
	    {
			if(current_error > 0 && current_error < 100)
			{
				set_motor(1, (int)50);
				set_motor(2, -50-speed);
			}
			else if(current_error < 0  && current_error > -100)
			{
				set_motor(1, 50-speed);
				set_motor(2, -50);
			}
			else if(read_analog(2)<400)
			{
				double temp_error = 0;
			
				if(current_error > 0)
				{
					temp_error = 500 - (read_analog(1)-20);
					set_motor(1, 50+temp_error);
					set_motor(2, -50);	
				}
				else
				{
					temp_error = 500 - read_analog(0);
					set_motor(1, 50);
					set_motor(2, -50-temp_error);
				}
			}
			else
			{
				if(current_error>0) //turn left
				{
					set_motor(1, -50);
					set_motor(2, -50);
				}
				else //turn right
				{
					set_motor(1, 50);
					set_motor(2, 50);
				}
				sleep1(0, 750000);
			}
			sleep1(0, 6000);
	    }
	    else if(count<10)
	    {
			set_motor(1, 0);
			set_motor(2, 0);
			sleep1(1, 0);
			count++;
	    }
	    else
	    {
			set_motor(1, 50);
			set_motor(2, -50);
			count = 0;
			sleep1(0, 6000);
		}
	}
}

//OUTPUT: SET wheel speed to zero //NOTE: is this code used? remove it?
void kill()
{
	set_motor(2, 0);
	set_motor(1, 0);
}

//RUN main method
int main()
{
	init(); //INIT rasberry pi components 
	sector1();
	sector2();
	sector4();
}
