/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022
 * Header for Lab1 Task1b
 * This main program turns on and off LEDs 1, 2, 3 and 4 in a periodic pattern. LED's turn off and on one at a time from right to left.
 */ 

#ifndef lab2.h?  //Header Guard
#define lab2.h

#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))

#define TIMER0_CTL (*((volatile uint32_t *)0x4003000C)) 
  
#define TIMER0_CFG (*((volatile uint32_t *)0x40030000))
  
#define TIMER0_TAMR (*((volatile uint32_t *)0x40030004)) 
  
  
#define TIMER0_TAILR (*((volatile uint32_t *)0x40030028)) 
  
#define TIMER0_RIS (*((volatile uint32_t *)0x4003001C))

#define TIMER0_ICR (*((volatile uint32_t *)0x40030024))

void Timer0A_Int_Handler();
void pattern();

void initialization_timer();
void PortF_init();

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))       //Set up RCGCPIO

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))      //Sets up and assigns GPIOs for Port F LED 3 and LED 4
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))      //Sets up and assigns GPIOs for Port N LED 1 and LED 2
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))



#endif //lab2.h header file


              