/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Main Program for Lab1 Task1b
 * This main program uses switches 1 and 2 on TI microcontroller board to turn off and on LEDs 3 and 4 on the board. 
 * Switch 1 was set to turn LED 3 on and Switch 2 to turn LED 4 on. 
 */ 


#include <stdint.h> 
#include "lab2.h"

#define LED4 0x1
#define LED3 0x10
#define LED2 0x1
#define LED1 0x2
#define SW1 0x1
#define SW2 0x2
   
int main(void){
  
 initialization_IO();
 initialization_timer();

 
  while(1) {
     
 
   
  }
  
  return 0;
 
}

void initialization_IO() {
  
  RCGCGPIO |= 0x1120; // Enable PortF, PortN, and PortJ GPIOs
  
  GPIODIR_F = 0x11; // Set PF4 and PF0 to output for LED 3 and 4
  GPIODEN_F = 0x11; // Set PF4 and PF0 to digital port for LED 3 and 4
  GPIODATA_F = 0x0; // Set PF4 and PF0 to 1 for LED 3 and 4
 
  GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LED 1 and 2
  GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LED 1 and 2
  
  GPIODIR_J = 0x0; // Set PJ0 and PJ1 to input for Switches 1 and 2
  GPIODEN_J = 0x3; //Set PJ0 and PJ1 to digital port for Switches 1 and 2
  GPIOPUR_J = 0x3;  // Attach pull-up resistors to pins PJ0 and PJ1 
  
  //Interrupts
 
  /*GPIO_PORTF_IS &= ~(LED3 | LED4);
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
  NVIC_EN2 |= 0x200 ;  //Enable Port N Interrupt number 73 */
  
  GPIO_PORTJ_IS &= ~(SW1 | SW2);
  GPIO_PORTJ_IBE &= ~(SW1 | SW2);
  GPIO_PORTJ_IEV = ~(SW1 | SW2);
  GPIO_PORTJ_ICR |= (SW1 | SW2);
  GPIO_PORTJ_IM |= (SW1 | SW2);
  GPIO_PORTJ_PRI = (SW1 | SW2); 
  NVIC_EN1 |= 0x80000 ;  //Enable Port J Interrupt number 51
}

void initialization_timer() {
  
  //Configure Timer
  RCGCTIMER = 0x01; //Enable Timer 0
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
  TIMER0_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  
  
  TIMER0_TAILR = 0x00F42400; //Timer set to 16000000
  
  //Interrupts
  NVIC_EN0 |= 0x80000; 
  TIMER0_IMR = 0x00000001;
  TIMER0_CTL |= 0x01; //Enable Timer
              
}

void Timer0A_Int_Handler() {
    
   TIMER0_ICR = 0x00000001;
   GPIODATA_N ^= LED1;
}

void PortF_Int_Handler() {
  
}

void PortN_Int_Handler() {
}

void PortJ_Int_Handler() {
  
  if (GPIO_PORTJ_MIS & SW1) {
     GPIODATA_N = LED2;
     TIMER0_IMR = 0x0;
    GPIO_PORTJ_ICR |= SW1;
    
   
  } else if (GPIO_PORTJ_MIS & SW2) {
    GPIODATA_N = LED1;
    TIMER0_IMR = 0x1;
    GPIO_PORTJ_ICR |= SW2;
    
    
  }
  
}