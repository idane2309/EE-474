 /**
 * EE/CSE 474: Lab3 Task1a main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
 * This main program takes a character input from the computer through the PUTTY terminal; The microcontroller recieves this character input via a
 * bluetooth module and then transmits the character back to the computer PUTTY terminal. The character then shows up on the putty terminal. 
 */



#include <stdint.h>
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include <stdio.h>


// STEP 0b: Include your header file here
#include "tm4c1294ncpdt.h"



int main(void) {
 
  LCD_Init(); 
  LCD_ColorFill(Color4[2]);
  
  while(1) {
    
  }
  
  
  return 0;
}



