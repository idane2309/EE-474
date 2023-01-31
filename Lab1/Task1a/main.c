#include <stdint.h>
#include "lab1.h"
int main(void)
{
 volatile unsigned short delay = 0;
 
 RCGCGPIO |= 0x1120; // Enable PortF GPIO
 delay++; // Delay 2 more cycles before access Timer registers
 delay++; // Refer to Page. 756 of Datasheet for info
 
 GPIODIR_F = 0x11; // Set PF0 to output
 GPIODEN_F = 0x11; // Set PF0 to digital port
 //GPIODATA_F = 0x11; // Set PF0 to 1
 
  GPIODIR_N = 0x3; // Set PF0 to output
 GPIODEN_N = 0x3; // Set PF0 to digital port
 //GPIODATA_N = 0x3; // Set PF0 to 1
 
   GPIODIR_J = 0x0; // Set PF0 to output
 GPIODEN_J = 0x3; // Set PF0 to digital port
 //GPIODATA_J = 0xFF;
 
 while (1) {
 
   for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x0; // Set PF0 to 1
      GPIODATA_N = 0x0; // Set PF0 to 1
  }
  
     for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x1; // Set PF0 to 1
      
  }
  
  for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x0; // Set PF0 to 1
      
  }
  
     for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x10; // Set PF0 to 1
     
  }

      for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x0; // Set PF0 to 1
      
  }
 
  for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x1; // Set PF0 to 1
  }
  
  for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x0; // Set PF0 to 1
  }
  
    for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x2; // Set PF0 to 1
  }
 
 }
 

 
 return 0;
 
}