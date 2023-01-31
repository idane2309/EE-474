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

enum TRAFF_States {idle, stop, warn, go} TRAFF_State;  //Create states with their assigned names. 

int main(void){
  
 Ports_init();
 initialization_timer0();
 initialization_timer1();
 
 TRAFF_State = idle;   //Set the initial state to idle
 
 
  while (1) {
    
    Traff_Latch();  //Includes void method with FSM function. Parameters include pushbuttons and LEDS
  
  }
    
  
  return 0;
 
}
  

//Function Traff_Latch sets up our Traffic System FSM with parameters for GPIODATA_L to manipulate LED outputs and GPIODATA_F AND GPIODATA_G
//for pushbutton inputs. These parameters are type uint32_t. The function is void so does not return any value.

void Traff_Latch() {
   switch(TRAFF_State) {  //Transitions
   
   case idle:                                  //Initial Idle State
      
    
     if ((GPIODATA_F & 0x8) && (TIMER0_CTL == 0x0)) { 
          Timer0_reinit();
         
       //THOUGHT STARTS HERE
          while (GPIODATA_F & 0x8)  {
            if (TIMER0_RIS & 0x1) {
              initialization_timer1();
              TRAFF_State = stop;
              break;
            }
          }
     } 
     break;
     //THOUGHT ENDS HERE
          
         
    
    case go:
      
      Timer1_Int_Handler();
      printf("Im at green\n");                                              //Start in Green light go state in Traffic system. 
      
      
      if (!(TIMER1_RIS & 0x1)) {
        
          //OFF STATEMENT 2 SECOND HOLD
         if ((GPIODATA_F & 0x8) && (TIMER0_CTL == 0x0)) { 
          Timer0_reinit();
      
          while (GPIODATA_F & 0x8)  {
            if (TIMER0_RIS & 0x1) {
              
              TRAFF_State = idle;
              break;
            }
          }
         } 
         
        //PEDESTRIAN STATEMENT 2 SECOND HOLD
         if ((GPIODATA_G & 0x1) && (TIMER0_CTL == 0x0)) { 
          Timer0_reinit();
         
              while (GPIODATA_G & 0x1)  {
                if (TIMER0_RIS & 0x1) {
                  initialization_timer1(); //REMOVED FOR NOW
                  TRAFF_State = warn;
                  break;
                }
              }
          } 
           
      } else {
        
        Timer1_Int_Handler();
        initialization_timer1();
        TRAFF_State = stop;                     //Else if no pushbutton pressed move to stop state with Red light to start alternating between them.
        break;
      }    
   
    break;
    
    case stop:                                  //Stop state with red light in traffic system.
      
      while (GPIODATA_F & 0x8) {}
      Timer1_Int_Handler(); //ADDED TO CLEAR BIT
      printf("im at stop\n");
      if (!(TIMER1_RIS & 0x1)) {
        
        //OFF STATEMENT 2 SECOND HOLD
         if ((GPIODATA_F & 0x8) && (TIMER0_CTL == 0x0)) { 
          Timer0_reinit();
      
          while (GPIODATA_F & 0x8)  {
            if (TIMER0_RIS & 0x1) {
              
              TRAFF_State = idle;
              break;
            }
          }
          
         
        
         } else if ((GPIODATA_G & 0x1) && (!(GPIODATA_F & 0x8))) {
          
          TRAFF_State = stop;                                        //If pedestrian pushbutton pressed on stop state do nothing
          break;
        }
      } else {
      
        Timer1_Int_Handler();
        initialization_timer1();
        TRAFF_State = go;                                          //If no pushbutton pressed, move to go state to alternate between stop and go lights.
        break;
      }
    break;
    
    
    case warn:
      
   
      
      if (!(TIMER1_RIS & 0x1)) {   
         if(GPIODATA_F & 0x8) {      
          TRAFF_State = idle;                    //If on/off pushbutton pressed, move to idle state as we 'turn off' system.
          break;
         }
      } else {
        Timer1_Int_Handler();
        initialization_timer1();
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




void initialization_timer0() {
  
  //Configure Timer
  RCGCTIMER |= 0x1; //Enable Timer 0 and Timer 1
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
  //TIMER0_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  TIMER0_TAMR = 0x1; // This is the one-shot that it only starts the timer when RIS is reset
  
 
  
  TIMER0_TAILR = 0x00F42400 * 2; //Timer set to 16000000
  
  
              
}
    
void initialization_timer1() {
  
  //Configure Timer
  RCGCTIMER |= 0x2;
  
  TIMER1_CTL = 0x0; //Verify Timer is not running
  
  TIMER1_CFG = 0x0; //Timer is 16 bit and split. Remove the other CFG for A
  
  TIMER1_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  

  TIMER1_TAILR = 0x00F42400 * 5; //Timer set to 16000000
  
  
  Timer1_ON();
              
} 

void Ports_init() {
   
 RCGCGPIO |= 0x0460;              // activate clock for Port L, F, G GPIOs
 
 
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
    //TRAFF_State = idle;   //Set the initial state to idle
}

void Timer0_Int_Handler() {  //2 second timer
  //while (!(TIMER0_RIS & 0x1)) {}
    TIMER0_ICR = 0x01;    
}
   
void Timer1_Int_Handler() {  //5 second timer
  //while (!(TIMER1_RIS & 0x1)) {}
    TIMER1_ICR = 0x01;    
}

void Timer0_reinit() {
   RCGCTIMER |= 0x1; //Enable Timer 0 and Timer 1
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
 
  TIMER0_TAMR = 0x1; // This is the one-shot that it only starts the timer when RIS is reset
  
 
  
  TIMER0_TAILR = 0x00F42400 * 2; //Timer set to 16000000
  
  TIMER0_CTL |= 0x01; //Enable Timer
  
  Timer0_Int_Handler();
}

void Timer0_ON() {
  TIMER0_CTL = 0x01;
}

void Timer0_OFF() {
  
  TIMER0_CTL = 0x0; //Disable Timer
}

void Timer1_ON() {
  
  TIMER1_CTL = 0x1; //Enable Timer
}

void Timer1_OFF() {
  TIMER1_CTL = 0x0;
}

void Ped_Req() {
    if (!((GPIODATA_G & 0x1) == 1)) {
      Timer0_Int_Handler();
    } else if (TIMER0_RIS & 0x1) {
      Timer0_Int_Handler();
      TRAFF_State = warn;
    }
}

void onoff_req() {
while (GPIODATA_F & 0x8) {
    if (TIMER0_RIS & 0x1) {
      GPIODATA_F = 0x0;
    }
  }
}