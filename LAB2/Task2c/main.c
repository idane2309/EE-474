/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/19/2022
 * Main Program for Lab 1 Task 2
 * This main program uses external buttons as inputs and LEDS as outputs to display a working Traffic light system with a pedestrian switch.
 * When the system is turned on by the on/off pushbutton, the system starts at the green light and then alternates between the green light and the red stop
 * light. If the pedestrian pushbutton is pressed on the green light, the green light then transitions to a yellow warning light then to a red light. 
 * If the pedestrian pushbutton is pressed on the red light, nothing happens. 
 */ 

#include <stdint.h> 
#include "lab2.h"
#include <stdio.h>


#define OFF_LED 0x0
#define GREEN_LED 0x10
#define YELLOW_LED 0x8
#define RED_LED 0x4
#define BUTTON_ON_OFF 0x8
#define BUTTON_PED 0x1


int main(void){
  
 
 Ports_init();
 initialization_timer1();
 Timer0_reinit();
 GPIODATA_L = OFF_LED;
 

 
 
  while (1) {
    
  }
    
  
  return 0;
 
}
  


void initialization_timer1() {
  
  //Configure Timer
  RCGCTIMER |= 0x2;
  
  TIMER1_CTL = 0x0; //Verify Timer is not running
  
  TIMER1_CFG = 0x0; //Timer is 16 bit and split. Remove the other CFG for A
  
  TIMER1_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  

  TIMER1_TAILR = 0x00F42400 * 5; //Timer set to 16000000
  
   //Interrupts
  NVIC_EN0 |= 0x200000; 
  TIMER1_IMR = 0x1;
 // TIMER1_CTL |= 0x01; //Enable Timer
  
              
} 

void Ports_init() {
   
 RCGCGPIO |= 0x0460;              // activate clock for Port L, F, G GPIOs
 
    //Port L Init
    GPIOAMSEL_L &= ~0x1C;          // disable analog function of PL2, PL3, PL4: LEDS
    GPIODIR_L |= 0x1C;             // set LEDS PL2, PL3, PL4 to output (1) 
    GPIOAFSEL_L &= ~0x1C;          // set PL2, PL3, PL4 regular port function 
    GPIODEN_L |= 0x1C;             // enable digital output on PL2, PL3, PL4
    
    
    //Port F Init
    GPIOAMSEL_F &= ~0x8;  // Disable PF3 analog function FOR PUSHBUTTON  ON/OFF
    GPIOAFSEL_F &= ~0x8;  // Select PF3 regular port function FOR PUSHBUTTON ON/OFF
    GPIODIR_F &= ~0x8;    // Set PF3 to input direction FOR PUSHBUTTON ON/OFF
    GPIODEN_F |= 0x8;     // Enable PF3 digital function FOR PUSHBUTTON ON/OFF
    
    //Interrupts for Port F
    GPIO_PORTF_IS &= (BUTTON_ON_OFF);
    GPIO_PORTF_IBE &= (BUTTON_ON_OFF);
    GPIO_PORTF_IEV = (BUTTON_ON_OFF);
    GPIO_PORTF_ICR |= BUTTON_ON_OFF;
    GPIO_PORTF_IM |= BUTTON_ON_OFF;
    GPIO_PORTF_PRI = BUTTON_ON_OFF; 
    NVIC_EN0 |= 0x40000000 ;  //Enable Port F Interrupt number 30
  
    //Port G Init
    GPIOAMSEL_G &= ~0x1;  // Disable PG0 analog function FOR PUSHBUTTON PEDESTRIAN
    GPIOAFSEL_G &= ~0x1;  // Select PG0 regular port function FOR PUSHBUTTON PEDESTRIAN
    GPIODIR_G &= ~0x1;    // Set PG0 to input direction FOR PUSHBUTTON PEDESTRIAN
    GPIODEN_G |= 0x1;     // Enable PG0 digital function FOR PUSHBUTTON PEDESTRIAN
    
    //Interrupts for Port G
    GPIO_PORTG_IS &= (BUTTON_PED);
    GPIO_PORTG_IBE &= (BUTTON_PED);
    GPIO_PORTG_IEV = (BUTTON_PED);
    GPIO_PORTG_ICR |= (BUTTON_PED);
    GPIO_PORTG_IM |= (BUTTON_PED);
    GPIO_PORTG_PRI = (BUTTON_PED); 
    NVIC_EN0 |= 0x80000000 ;  //Enable Port G Interrupt number 31
  
    //GPIODATA_L = OFF_LED;     //Initialize the LEDS to off state
    //TRAFF_State = idle;   //Set the initial state to idle
} 

void PortF_Int_Handler() {
  
  
  
  if ((GPIO_PORTF_MIS & BUTTON_ON_OFF) && (TIMER0_CTL == 0x0)) {
    Timer0_reinit();
    GPIO_PORTF_ICR = BUTTON_ON_OFF;
  }  
  
}

void PortG_Int_Handler() {
  
  if ((GPIO_PORTG_MIS & BUTTON_PED) && (TIMER0_CTL == 0x0)) {   //1 Down Rising edge, let go of button 0 Falling Edge
    Timer0_reinit();
    GPIO_PORTG_ICR = BUTTON_PED;                              //USE EDGE SENSITIVE REGISTER SECTION 10.4 STEP 10.
  } 
  
}

void Timer0A_Int_Handler() {  //2 second timer
    
    TIMER0_ICR = 0x01;
    if(GPIODATA_G & BUTTON_PED) {
      initialization_timer1();
      if (GPIODATA_L & GREEN_LED) {
        GPIODATA_L = YELLOW_LED;    
        TIMER1_CTL = 0x1; //ADDED
        TIMER0_CTL = 0x0;
      }
    } 
    
    //ON OFF SHIT STARTS HERE
    if(GPIODATA_F & BUTTON_ON_OFF) {
      if (GPIODATA_L == OFF_LED) {
        GPIODATA_L = RED_LED;
        Timer1_ON();
        TIMER0_CTL = 0x0;
      } else if (!(GPIODATA_L & OFF_LED)) {
        GPIODATA_L = OFF_LED;
        initialization_timer1();
        TIMER0_CTL = 0x0;
      } 
    }
    //Ends here 
    TIMER0_CTL = 0x0;
}
   
void Timer1A_Int_Handler() {  //5 second timer
  
    TIMER1_ICR = 0x01; 
    
    if (GPIODATA_L & RED_LED) {
      GPIODATA_L = GREEN_LED;
    } else if (GPIODATA_L & GREEN_LED) {
      GPIODATA_L = RED_LED;
    
    } else if ((GPIODATA_L & YELLOW_LED)) { 
      GPIODATA_L = RED_LED;
    } 
    
    //if ((GPIODATA_G & BUTTON_PED) && (GPIODATA_L & YELLOW_LED)) {   //ADDED ALL THIS
      
    // if ((!(GPIODATA_G & BUTTON_PED)) && (GPIODATA_L & YELLOW_LED)) {
    //  GPIODATA_L = RED_LED;
    //}
   
}

void Timer0_reinit() {
   RCGCTIMER |= 0x1; //Enable Timer 0 and Timer 1
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
  TIMER0_TAMR = 0x1; // This is the one-shot that it only starts the timer when RIS is reset
  
  TIMER0_TAILR = 0x00F42400 * 2; //Timer set to 16000000
  
  //Interrupts
  NVIC_EN0 |= 0x80000; 
  TIMER0_IMR = 0x1;
  TIMER0_CTL |= 0x01; //Enable Timer
  
  TIMER0_ICR = 0x01;
}

void Timer0_ON() {
  TIMER0_CTL = 0x1;
}

void Timer1_ON() {
  TIMER1_CTL = 0x1;
}



