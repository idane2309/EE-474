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
#include "lab1.h"

enum TRAFF_States {idle, stop, warn, go} TRAFF_State;  //Create states with their assigned names. 


//Function Traff_Latch sets up our Traffic System FSM with parameters for GPIODATA_L to manipulate LED outputs and GPIODATA_F AND GPIODATA_G
//for pushbutton inputs. These parameters are type uint32_t. The function is void so does not return any value.
void Traff_Latch(uint32_t L, uint32_t F, uint32_t G) {
  
  switch(TRAFF_State) {  //Transitions
    case idle:                                  //Initial Idle State
      for(int j = 0; j <3000000; j++) {}        //Delay created by for loop
      
      if(GPIODATA_F & 0x8) {               
        TRAFF_State = go;                        //First Transition to Green light from on/off pushbutton as we 'turn on' system
        break;
      } else {
        TRAFF_State = idle;                     //If pushbutton not pressed, state stays on idle
      }                                               
    break;
    
    case go:                                    //Start in Green light go state in Traffic system. 
      for(int j = 0; j <3000000; j++) {}        //Delay created by for loop
      if(GPIODATA_F & 0x8) {                    
        TRAFF_State = idle;                     //If on/off pushbutton pressed, move back to idle state as we are 'turning off' system
        break;
      } else if ((GPIODATA_G & 0x1) == 0x1) {   
        TRAFF_State = warn;                     //If pedestrian pushbutton pressed, move to warn state with Yellow light
        break;
      } else {
        TRAFF_State = stop;                     //Else if no pushbutton pressed move to stop state with Red light to start alternating between them.
        break;
      }    
    break;
    
    
    case stop:                                  //Stop state with red light in traffic system.
      for(int j = 0; j <3000000; j++) {}        //Delay created with for loop
       if(GPIODATA_F & 0x8) {
        TRAFF_State = idle;                     //If on/off pushbutton pressed, move to idle state as we 'turn off' system.
        break;
      } else if ((GPIODATA_G & 0x1) && (GPIODATA_F & ~0x8)) {
        TRAFF_State = stop;                                        //If pedestrian pushbutton pressed on stop state do nothing
        break;
      } else {
        TRAFF_State = go;                                          //If no pushbutton pressed, move to go state to alternate between stop and go lights.
        break;
      }
    break;
    
    
    case warn:
      for(int j = 0; j <3000000; j++) {}       //Delay created by for loop
       if(GPIODATA_F & 0x8) {
        TRAFF_State = idle;                    //If on/off pushbutton pressed, move to idle state as we 'turn off' system.
        break;
      
      } else {
        TRAFF_State = stop;                    //If on/off pushbutton not pressed, carry on in sequence: move to stop state to complete pedestrian warning.
        break;
      } 
      break;
      
    
      
  } //Transitions
  
  switch(TRAFF_State) { //State Actions
    case idle: 
      GPIODATA_L = 0x0;        //Turn off LEDS in idle state
      break;
    
    case go:
      GPIODATA_L = 0x10;       //Set green LED on in go state
      break;
      
    case warn:
      GPIODATA_L = 0x8;         //Set yellow LED on in warn state
      break;
      
    case stop: 
      GPIODATA_L = 0x4;         //Set red LED on in stop state
      break;
    
  }
    
}






int main(void){
  
   volatile unsigned short delay = 0; 
    RCGCGPIO |= 0x0460;              // activate clock for Port L, F, G GPIOs
    delay++;                         // Delay 2 more cycles before access Timer registers
    delay++; 
 
    GPIOAMSEL_L &= ~0x1C;          // disable analog function of PL2, PL3, PL4: LEDS
    GPIODIR_L |= 0x1C;             // set LEDS PL2, PL3, PL4 to output (1) 
    GPIOAFSEL_L &= ~0x1C;          // set PL2, PL3, PL4 regular port function 
    GPIODEN_L |= 0x1C;             // enable digital output on PL2, PL3, PL4
    
    
    
    GPIOAMSEL_F &= ~0x8;  // Disable PF3 analog function FOR PUSHBUTTON  ON/OFF
    GPIOAFSEL_F &= ~0x8;  // Select PF3 regular port function FOR PUSHBUTTON ON/OFF
    GPIODIR_F &= ~0x8;    // Set PF3 to input direction FOR PUSHBUTTON ON/OFF
    GPIODEN_F |= 0x8;     // Enable PF3 digital function FOR PUSHBUTTON ON/OFF
    
    GPIOAMSEL_G &= ~0x1;  // Disable PG0 analog function FOR PUSHBUTTON PEDESTRIAN
    GPIOAFSEL_G &= ~0x1;  // Select PG0 regular port function FOR PUSHBUTTON PEDESTRIAN
    GPIODIR_G &= ~0x1;    // Set PG0 to input direction FOR PUSHBUTTON PEDESTRIAN
    GPIODEN_G |= 0x1;     // Enable PG0 digital function FOR PUSHBUTTON PEDESTRIAN
    
  
    GPIODATA_L = 0x0;     //Initialize the LEDS to off state
    TRAFF_State = idle;   //Set the initial state to idle
    
    
  while (1) {
    
    Traff_Latch(GPIODATA_L, GPIODATA_F, GPIODATA_G);  //Includes void method with FSM function. Parameters include pushbuttons and LEDS
  
  }
    
  
  return 0;
 
}
