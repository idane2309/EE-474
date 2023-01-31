
#ifndef lab2.h? // header guards 
#define lab2.h

#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))


//Timer 0 
#define TIMER0_CTL (*((volatile uint32_t *)0x4003000C)) 
  
#define TIMER0_CFG (*((volatile uint32_t *)0x40030000))
  #define TIMER0_ICR (*((volatile uint32_t *)0x40030024))

#define TIMER0_TAMR (*((volatile uint32_t *)0x40030004)) 
  
  
#define TIMER0_TAILR (*((volatile uint32_t *)0x40030028)) 
  
#define TIMER0_RIS (*((volatile uint32_t *)0x4003001C))


#define TIMER0_VAL (*((volatile uint32_t *)0x40030050))


//Timer 1
#define TIMER1_CTL (*((volatile uint32_t *)0x4003100C)) 
  
#define TIMER1_CFG (*((volatile uint32_t *)0x40031000))
  
#define TIMER1_TAMR (*((volatile uint32_t *)0x40031004)) 
  
  
#define TIMER1_TAILR (*((volatile uint32_t *)0x40031028)) 
  
#define TIMER1_RIS (*((volatile uint32_t *)0x4003101C))

#define TIMER1_ICR (*((volatile uint32_t *)0x40031024))

void Timer0_Int_Handler();
void Timer1_Int_Handler();
void initialization_timer0();
void initialization_timer1();
void Ports_init();
void Timer0_reinit();
void Timer1_ON();
void Timer0_OFF();
void Timer0_ON();
void Timer1_OFF();

void Ped_Req();

#define RCGCGPIO         (*((volatile uint32_t *)0x400FE608)) //Assign RCGCPIO

#define GPIOAMSEL_L      (*((volatile uint32_t *)0x40062528)) //Set up and Assign Port L GPIOs for LEDS
#define GPIODIR_L        (*((volatile uint32_t *)0x40062400))  
#define GPIODEN_L        (*((volatile uint32_t *)0x4006251C)) 
#define GPIOAFSEL_L      (*((volatile uint32_t *)0x40062420)) 
#define GPIODATA_L       (*((volatile uint32_t *)0x400623FC)) 

#define GPIOAMSEL_F      (*((volatile uint32_t *)0x4005D528)) //Set up and Assign Port F GPIOs for Pushbutton 1
#define GPIODIR_F        (*((volatile uint32_t *)0x4005D400))  
#define GPIODEN_F        (*((volatile uint32_t *)0x4005D51C)) 
#define GPIOAFSEL_F      (*((volatile uint32_t *)0x4005D420)) 
#define GPIODATA_F       (*((volatile uint32_t *)0x4005D3FC)) 

#define GPIOAMSEL_G      (*((volatile uint32_t *)0x4005E528)) //Set up and Assign Port G GPIOs for Pushbutton 2
#define GPIODIR_G        (*((volatile uint32_t *)0x4005E400))  
#define GPIODEN_G        (*((volatile uint32_t *)0x4005E51C)) 
#define GPIOAFSEL_G      (*((volatile uint32_t *)0x4005E420)) 
#define GPIODATA_G       (*((volatile uint32_t *)0x4005E3FC)) 


//Function Traff_Latch sets up the FSM on C with inputs provided by Pushbuttons PG0 and PF3 and LEDS PL2, PL3, PL4
void Traff_Latch();  //Assign in header to tell compiler to use function in main.

#endif //lab2.h HEADER FIle

 