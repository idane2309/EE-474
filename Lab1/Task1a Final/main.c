/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Main Program for Lab1 Task1b
 * This main program turns on and off LEDs 1, 2, 3 and 4 in a periodic pattern. LED's turn off and on one at a time from right to left.
 */ 


#include <stdint.h>
#include "lab1.h"

int main(void)
{
 volatile unsigned short delay = 0;
 
 RCGCGPIO |= 0x1120; // Enable PortF and PortN GPIOs
 delay++; // Delay 2 more cycles before access Timer registers
 delay++; 
 
 GPIODIR_F = 0x11; // Set PF0 and PF4 to output for LEDs 3 and 4
 GPIODEN_F = 0x11; // Set PF0 and PF4 to digital port for LEDs 3 and 4
 
  GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LEDs 1 and 2
 GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LEDS 1 and 2
 
 
 while (1) {
 
   for (int j = 0; j < 1000000; j++){   //For loops used to create delay
     GPIODATA_F = 0x0; // Set LED 3 and 4 to O
      GPIODATA_N = 0x0; // Set LED 1 and 2 to 0
  }
  
     for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x1; // Set LED 3 to 1
      
  }
  
  for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x0; // Set LED 3 to 0
      
  }
  
     for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x10; // Set LED 4 to 1
     
  }

      for (int j = 0; j < 1000000; j++){
     GPIODATA_F = 0x0; // Set LED 4 to 0
      
  }
 
  for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x1; // Set LED 1 to 1
  }
  
  for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x0; // Set LED 1 to 0
  }
  
    for (int j = 0; j < 1000000; j++){
     
      GPIODATA_N = 0x2; // Set LED 2 to 1
  }
 
 }
 

 
 return 0;
 
}