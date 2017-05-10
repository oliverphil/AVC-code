#include <stdio.h>
#include <time.h>
#include "E101.h"

int main()
{
	init();	
}

void sector2(){
  int previous_error = 0;
  while(true){
    take_picture()
    int proportional_signal = proportional_signal();
    previous_error, int derivative_signal = derivative_signal(previous_error);
    int total_error = proportional_signal + derivative_signal;
    set_motor(1, 80 + total_error);
    set_motor(2, -80 + total_error);
  }
}

int proportional_signal(){
  int current_error = 0;
    double kp = 0.5;
    int proportional_signal;
    for (i=0; i<320, i++){
      int error = (i-160)*get_pixel(i, 120, 3);
      current_error += error;
    }
    proportional_signal = error*kp;
  return proportional_signal;
}

int, int derivative_signal(int prev){
  int previous_error = prev;
  int current_error = 0;
  int kd = 5;
  int derivative_signal;
  for (i=0; i<320; i++){
    int error = (i-160)*get_pixel(i, 120, 3);
    current_error += error;
  }
  sleep1(0, 100000);
  derivative_signal = (current_error-previous_error*10)*kd;
  previous_error = current_error;
  return previous_error, derivative_signal;
}
  
