/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Main Program for Lab1 Task1b
 * This main program uses switches 1 and 2 on TI microcontroller board to turn off and on LEDs 3 and 4 on the board. 
 * Switch 1 was set to turn LED 3 on and Switch 2 to turn LED 4 on. 
 */ 

#include <stdint.h> 
#include "lab1.h"

int main(void){
  volatile unsigned short delay = 0;
 
  RCGCGPIO |= 0x1120; // Enable PortF, PortN, and PortJ GPIOs
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; 
 
  GPIODIR_F = 0x11; // Set PF4 and PF0 to output for LED 3 and 4
  GPIODEN_F = 0x11; // Set PF4 and PF0 to digital port for LED 3 and 4
  GPIODATA_F = 0x0; // Set PF4 and PF0 to 1 for LED 3 and 4
 
  GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LED 1 and 2
  GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LED 1 and 2
  
 
  GPIODIR_J = 0x0; // Set PJ0 and PJ1 to input for Switches 1 and 2
  GPIODEN_J = 0x3; //Set PJ0 and PJ1 to digital port for Switches 1 and 2
  GPIOPUR_J = 0x3;  // Attach pull-up resistors to pins PJ0 and PJ1 
 
  while(1) {
     
    if(~GPIODATA_J & 0x1) {            
      GPIODATA_N = GPIODATA_J;         //If Switch 1 is pressed (Active Low), Turns on LED 3
    } else if (~GPIODATA_J & 0x2) {
      GPIODATA_N = GPIODATA_J;         //If Switch 2 is pressed (Active Low), Turns on LED 4
    } else  {
      GPIODATA_N = 0x0;                //If none of the switches are pressed, Keep LED 3 and LED 4 off
    }
   
  }
  
  return 0;
 
}