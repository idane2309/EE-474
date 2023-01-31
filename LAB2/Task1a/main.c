/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Main Program for Lab1 Task1b
 * This main program turns on and off LEDs 1, 2, 3 and 4 in a periodic pattern. LED's turn off and on one at a time from right to left.
 */ 


#include <stdint.h>
#include "lab2.h"

int main(void)
{
 
 PortF_init();
 initialization_timer();
 
 while (1) {
  pattern();
 }
 
 return 0;
 
}

void initialization_timer() {
  
  //Configure Timer
  RCGCTIMER = 0x01; //Enable Timer 0
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
  TIMER0_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  
 // TIMER0_TAMR = ; //Timer count down (TACDIR BIT)
  
  TIMER0_TAILR = 0x00F42400; //Timer set to 16000000
  
  TIMER0_CTL |= 0x01; //Enable Timer
              
}

void PortF_init() {
   
 RCGCGPIO |= 0x1120; // Enable PortF and PortN GPIOs
 
 
 GPIODIR_F = 0x11; // Set PF0 and PF4 to output for LEDs 3 and 4
 GPIODEN_F = 0x11; // Set PF0 and PF4 to digital port for LEDs 3 and 4
 
  GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LEDs 1 and 2
 GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LEDS 1 and 2
 
 //GPIODATA_F = 0x0; // Set LED 3 and 4 to O
   //   GPIODATA_N = 0x0; // Set LED 1 and 2 to 0
 
}

void Timer0A_Int_Handler() {
    while (!(TIMER0_RIS & 0x1)) {}
    TIMER0_ICR |= 0x01;    
}

void pattern() {
 
 
  Timer0A_Int_Handler();
 GPIODATA_N = 0x0;
  
 Timer0A_Int_Handler();
  GPIODATA_F = 0x1;
 
  Timer0A_Int_Handler();
  GPIODATA_F = 0x0;

  Timer0A_Int_Handler();
  GPIODATA_F = 0x10;
 
  Timer0A_Int_Handler();
  GPIODATA_F = 0x0;
 
  Timer0A_Int_Handler();
  GPIODATA_N = 0x1;

  Timer0A_Int_Handler();
  GPIODATA_N = 0x0;
 
  Timer0A_Int_Handler();
  GPIODATA_N = 0x2;
 
  
}