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

#define TIMER0_IMR (*((volatile uint32_t *)0x40030018))  //IMR for interrupt. Offset = 0x018

#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))  //NVIC EN0 interrupt offset = 0x100, Base = 0xE000E000

#define NVIC_EN2 (*((volatile uint32_t *)0xE000E108))  //NVIC EN0 interrupt offset = 0x100, Base = 0xE000E000

//Port F Interrupts
#define GPIO_PORTF_IS (*((volatile uint32_t *)0x4005D404))
#define  GPIO_PORTF_IBE (*((volatile uint32_t *)0x4005D408))
#define  GPIO_PORTF_IEV (*((volatile uint32_t *)0x4005D40C))
#define  GPIO_PORTF_ICR (*((volatile uint32_t *)0x4005D41C))
#define  GPIO_PORTF_IM (*((volatile uint32_t *)0x4005D410))
#define  GPIO_PORTF_PRI (*((volatile uint32_t *)0xE000E41C)) //Priority Register
#define  GPIO_PORTF_MIS (*((volatile uint32_t *)0x4005D418)) //MIS


//Port N Interrupts
#define GPIO_PORTN_IS (*((volatile uint32_t *)0x40064404))
#define  GPIO_PORTN_IBE (*((volatile uint32_t *)0x40064408))
#define  GPIO_PORTN_IEV (*((volatile uint32_t *)0x4006440C))
#define  GPIO_PORTN_ICR (*((volatile uint32_t *)0x4006441C))
#define  GPIO_PORTN_IM (*((volatile uint32_t *)0x40064410))
#define  GPIO_PORTN_PRI (*((volatile uint32_t *)0xE000E448)) //Priority Register for Interrupt 73
#define  GPIO_PORTN_MIS (*((volatile uint32_t *)0x40064418)) //MIS

void Timer0A_Int_Handler();
void initialization_IO();
void PortF_Int_Handler();
void initialization_timer();
void PortN_Int_Handler();

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))       //Set up RCGCPIO

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))      //Sets up and assigns GPIOs for Port F LED 3 and LED 4
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))      //Sets up and assigns GPIOs for Port N LED 1 and LED 2
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))



#endif //lab2.h header file


              