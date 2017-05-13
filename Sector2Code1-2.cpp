//AVC Challenge
//Team:	BTN

//Quadrant Name:	Curved Line
//Quadrant Number:	2 of 4

//IMPORT libraries
#include <stdio.h>
#include <time.h>
#include "E101.h"

//INIT global variables
int previous_error = 0;
int current_error = 0;
int num_white_pixels = 0;

//CALCULATE proportional_signal
int GET_proportional_signal(){
	printf("GET Proportional signal\n"); //user feedback
 	
	//SET global variables
	current_error = 0;
	num_white_pixels = 0;
    	
	//INIT local variables
	int proportional_signal;
	double kp = 0.0005; //NOTE: should this we INIT as a GLOBAL variable? so we can change it from the top on the fly during testing?
    	
	//CALCULATE current_error
	for (int i=0; i<320; i++){ //FOR every pixel in the frame
     		
		//INIT local variables
		int pixel = get_pixel(i, 120, 3); //NOTE: what does the 120 stand for? halfway? could this be grabbed with a "frame.length()/2"?
      		int error = (i-160)*pixel;	//NOTE: what does the 160 represent?
      		
		if (pixel>127){ //NOTE: should this 127 be grabbed from an INIT variable "color_range/2" or "max/2" for clarity?
			num_white_pixels++;
      		}
		current_error += error;
    	}
	proportional_signal = current_error*kp;
    	return proportional_signal;
}

//CALCULATE derivative signal
int GET_derivative_signal(){
	printf("GET Derivative signal\n"); //user feedback
  	
	//SET global variables
	current_error = 0;
	
	//INIT local variables
	int derivative_signal;
  	int kd = 0.0005;
	
	//CALCULATE current_error
	for (int i=0; i<320; i++){ //FOR every pixel in the frame
    		
		//INIT local variables
		int pixel = get_pixel(i, 120, 3);
		int error = (i-160)*get_pixel(i, 120, 3);
    		
		current_error += error;
  	}
  	//DELAY the program
	sleep1(0, 100000); //NOTE: should we INIT a GLOBAL sleep variable so we can change it if necassary?
  	
	derivative_signal = (current_error-previous_error*10)*kd;
	previous_error = current_error;
  	return derivative_signal;
}

//CALCULATE integral signal

void sector2(){
  	while(true){
		printf("sector2\n"); //NOTE: won't this print repeatedly? should this be moved to above the "while(true)" statement?
		
		//SET global variables
	  	num_white_pixels = 0; //NOTE: is this supposed to be here? won't this be SET in the GET_prop_signal method?
       
		if(num_white_pixels<128){ //NOTE: this will never break as the comparator is SET just before?
    			
			//INPUT: GET frame from camera component
			take_picture(); //NOTE: can this be assigned to a frame variable? can we pull a ".length()" from it?
			
			//SET global variables
			previous_error = 0;
			
			//INIT local variables
			int proportional_signal = GET_proportional_signal();
    			int derivative_signal = GET_derivative_signal();
			//
			int total_error = proportional_signal + derivative_signal;
			printf("%d\n", total_error); //user feedback
    			
			//OUTPUT: SET wheel speed
			set_motor(1, 80 + total_error); //NOTE: what is the "80" stand for? should this we INIT as a GLOBAL speed variable?
			set_motor(2, -80 + total_error);
       		}
		else{
			break; //NOTE: what does this mean? break the code when a full line is detected? What if broken accidently?
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
