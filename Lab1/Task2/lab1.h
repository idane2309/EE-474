
#ifndef lab1.h? // header guards 
#define lab1.h

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
void Traff_Latch(uint32_t L, uint32_t F, uint32_t G);  //Assign in header to tell compiler to use function in main.

#endif

 