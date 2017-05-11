
#include <stdio.h>
#include <time.h>
#include "E101.h"
int previous_error = 0;
int numberWhite = 0;

int proportional_signal(){
	printf("Proportional\n");
  int current_error = 0;
    double kp = 0.0005;
    int proportionalSignal;
    numberWhite = 0;
    for (int i=0; i<320; i++){
      int pixel = get_pixel(i, 120, 3);
      int error = (i-160)*pixel;
      if (pixel>127){
		numberWhite++;
      }
      current_error += error;
    }
    proportionalSignal = current_error*kp;
    
    return proportionalSignal;
}

int derivative_signal(){
	printf("Derivative\n");
  int current_error = 0;
  int kd = 0.0005;
  int derivative_signal;
  for (int i=0; i<320; i++){
    int error = (i-160)*get_pixel(i, 120, 3);
    current_error += error;
  }
  sleep1(0, 100000);
  derivative_signal = (current_error-previous_error*10)*kd;
  previous_error = current_error;
  return derivative_signal;
}

void sector2(){

  while(true){
	  printf("sector2\n");
    numberWhite = 0;
       if(numberWhite<128){
    	take_picture();
    	int proportionalSignal = proportional_signal();
    	previous_error = 0;
    	int derivativeSignal = derivative_signal();
    	int total_error = proportionalSignal + derivativeSignal;
    	printf("%d\n", total_error);
    	set_motor(1, 80 + total_error);
    	set_motor(2, -80 + total_error);
       } else {
	  break;
       }
  }
}

void kill(){
	set_motor(2, 0);
	set_motor(1, 0);
}

int main()
{
	init();	
    sector2();
}
