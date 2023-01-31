/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Main Program for Lab1 Task1b
 * This main program turns on and off LEDs 1, 2, 3 and 4 in a periodic pattern. LED's turn off and on one at a time from right to left.
 */ 

#include <stdint.h>
#include "lab2.h"

#define LED4 0x1
#define LED3 0x10
#define LED2 0x1
#define LED1 0x2


int main(void)
{
 
 initialization_IO();
 initialization_timer();

 GPIODATA_F = LED4;
 
 while (1) {
 
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
  
  //Interrupts
  NVIC_EN0 |= 0x80000; 
  TIMER0_IMR = 0x00000001;
  TIMER0_CTL |= 0x01; //Enable Timer
  
  
              
}

void initialization_IO() {
   
 RCGCGPIO |= 0x1120; // Enable PortF and PortN GPIOs
 
 GPIODIR_F = 0x11; // Set PF0 and PF4 to output for LEDs 3 and 4
 GPIODEN_F = 0x11; // Set PF0 and PF4 to digital port for LEDs 3 and 4
 
  GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LEDs 1 and 2
 GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LEDS 1 and 2
 
 //GPIODATA_F = 0x0; // Set LED 3 and 4 to O
   //   GPIODATA_N = 0x0; // Set LED 1 and 2 to 0
 
 //Interrupts
 
  GPIO_PORTF_IS &= ~(LED3 | LED4);
  GPIO_PORTF_IBE &= ~(LED3 | LED4);
  GPIO_PORTF_IEV = ~(LED3 | LED4);
  GPIO_PORTF_ICR |= (LED3 | LED4);
  GPIO_PORTF_IM |= (LED3 | LED4);
  GPIO_PORTF_PRI = (LED3 | LED4); 
  NVIC_EN0 |= 0x40000000 ;  //Enable Port F Interrupt number 30
  
  GPIO_PORTN_IS &= ~(LED1 | LED2);
  GPIO_PORTN_IBE &= ~(LED1 | LED2);
  GPIO_PORTN_IEV = ~(LED1 | LED2);
  GPIO_PORTN_ICR |= (LED1 | LED2);
  GPIO_PORTN_IM |= (LED1 | LED2);
  GPIO_PORTN_PRI = (LED1 | LED2); 
  NVIC_EN2 |= 0x200 ;  //Enable Port N Interrupt number 73
 
}

void Timer0A_Int_Handler() {
    
   TIMER0_ICR = 0x00000001;
  
   
   
  if (GPIODATA_F & LED4) {
    GPIODATA_F = LED3;
  } else if (GPIODATA_F & LED3) {
    GPIODATA_F = 0x0;
    GPIODATA_N = LED2;
  } else if (GPIODATA_N & LED2) {
    GPIODATA_N = LED1;
  } else if (GPIODATA_N & LED1) {
    GPIODATA_N = 0x0;
    GPIODATA_F = LED4;
  }
  
  
  
  
}

void PortF_Int_Handler() {
  
}

void PortN_Int_Handler() {
}
