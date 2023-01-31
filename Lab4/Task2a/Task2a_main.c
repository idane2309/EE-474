/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/2/2022
 * Main Program for Lab 2 Task 2c
 * This main program uses external buttons as inputs and LEDS as outputs to display a working Traffic light system with a pedestrian switch.
 * When the system is turned on by the on/off pushbutton, the system starts at the green light and then alternates between the green light and the red stop
 * light. If the pedestrian pushbutton is pressed on the green light, the green light then transitions to a yellow warning light then to a red light. 
 * If the pedestrian pushbutton is pressed on the red light, nothing happens. 
 */ 

#include <stdint.h> 
#include "Task2a_h.h"
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include <stdio.h>

#include "tm4c1294ncpdt.h"



//Definitions for LEDs and Buttons
#define OFF_LED 0x0
#define GREEN_LED 0x10
#define YELLOW_LED 0x8
#define RED_LED 0x4
#define BUTTON_ON_OFF 0x8
#define BUTTON_PED 0x1


int LED_COLOR = 0;
int reset = 0;
int ped = 0;

int main(void){
  
 
  LCD_Init();                //Initializes LCD Screen
  Touch_Init();              //Initializes Touch Screen
  
 //GPIODATA_L = OFF_LED;       //Initial State of the LEDs set to OFF.
  LCD_DrawCircle(50,60,45, Color4[10]);
  LCD_DrawCircle(160,60,45, Color4[14]);
  LCD_DrawCircle(270,60,45, Color4[12]);
    
  LCD_DrawCircle(90,180,45,Color4[15]); 
  LCD_SetCursor(70,180);
  LCD_Printf("ON | OFF");
  LCD_SetCursor(210,180);
  LCD_Printf("PEDESTRIAN");
  LCD_DrawCircle(240,180,45, Color4[15]);
  
  initialization_timer1();    //Initializes Timer 1 (5 second timer)
  Timer1_ON();
  Timer0_reinit();            //Initializes Timer 0 (2 second timer)

  while (1) {   
    
    if (Touch_ReadY() > 600 && Touch_ReadY() < 3000) {   
      if (Touch_ReadX() > 1600) {                       //PED BUTTON
        Timer0_ON();
        ped = ped + 1;
      }
      if (Touch_ReadX() < 1500) {                       //ON/OFF BUTTON
        Timer0_ON();
        reset = reset + 1;
      }
    } else {
      reset = 0;
      ped = 0;
    }
    
  }
    
  return 0;
 
}
  

//Initializes 5 second timer: Timer 1
void initialization_timer1() {
  
  //Configure Timer
  RCGCTIMER |= 0x2;
  
  TIMER1_CTL = 0x0; //Verify Timer is not running
  
  TIMER1_CFG = 0x0; //Timer is 32 bit
  
  TIMER1_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR BIT)
  

  TIMER1_TAILR = 0x00F42400 * 5; //Timer set to 16000000*5 = 5 seconds
  
  //Interrupts Configuration
  NVIC_EN0 |= 0x200000; 
  TIMER1_IMR = 0x1;
 
} 

//This handler method controls Timer 0's (2 second timer) interrupt. When Timer 0 interrupts the system, this handler method then check whether the 
//ON/OFF button is still being pressed when the 2 second timer is over; or if the pedestrian button is still being pressed when the 2 second timer is over.
//If either of these cases are true, the handler will go into their specific logic: For the ON/OFF button, the handler checks whether the system if on or off and turns on the 
//RED LED or turns off all LEDs based on this information. For the pedestrian button, the handler checks if the system is in the GREEN LED state and if true, then transitions
//to the YELLOW LED state and restarts the 5 second timer.

void Timer0A_Int_Handler() {  //2 second timer Handler
    
    TIMER0_ICR = 0x01;                  //Clears Timer Flag
    
    if (Touch_ReadY() > 600 && Touch_ReadY() < 1500) {   
      initialization_timer1();
      if (ped > 0) {                       //PED BUTTON
        if(LED_COLOR == 1) {
          LCD_DrawFilledCircle(50,60,44, Color4[0]);
          LCD_DrawFilledCircle(270,60,44, Color4[0]); 
          LCD_DrawFilledCircle(160,60,44, Color4[14]);
          LED_COLOR = 2;
          Timer1_ON();
          TIMER0_CTL = 0x0;
        }
      }
      
      if (reset > 0) {                       //ON/OFF BUTTON
        if (LED_COLOR == 0) {
          LCD_DrawFilledCircle(270,60,45,Color4[12]);
          Timer1_ON();
          LED_COLOR = 3;
          TIMER0_CTL = 0x0;
        } else if (LED_COLOR != 0) {
          LCD_DrawFilledCircle(50,60,44, Color4[0]);
          LCD_DrawFilledCircle(160,60,44, Color4[0]);
          LCD_DrawFilledCircle(270,60,44, Color4[0]); 
          LED_COLOR = 0;
          initialization_timer1();
          TIMER0_CTL = 0x0;
        }
      }
    }
    
    Timer0_reinit();
    TIMER0_CTL = 0x0;                   //Turns off 2 second timer if after either cases as a precaution that the timer is definitely off for reset.
}

//This handler method controls Timer 1's (5 second timer) interrupt. When Timer 1 interrupt the system, this handler method then clears the timer flag and checks
//the color state the LEDs are at. Based on the current color state, the handler will then transition between RED and GREEN or YELLOW periodically (checks every 5 seconds to keep LEDs on for 5 seconds). 
void Timer1A_Int_Handler() {  //5 second timer
  
    TIMER1_ICR = 0x01;          //Clears Timer 1 Flag
    
    if (LED_COLOR == 3) {
      LCD_DrawFilledCircle(160,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[0]);
      LCD_DrawFilledCircle(50,60,44,Color4[10]);           //Changes to GREEN LED if currently at RED LED
      LED_COLOR = 1;
    } else if (LED_COLOR == 1) {
      LCD_DrawFilledCircle(50,60,44,Color4[0]);             //Changes to RED LED if currently at GREEN LED
      LCD_DrawFilledCircle(160,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[12]);
      LED_COLOR = 3;
    
    } else if ((LED_COLOR == 2)) { 
      LCD_DrawFilledCircle(50,60,44,Color4[0]);             //Changes to RED LED if currently at YELLOW LED
      LCD_DrawFilledCircle(160,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[12]);
      LED_COLOR = 3;
    } 
    
}

//This method re-initializes the 2 second timer: Timer 0, and enables it.
void Timer0_reinit() {
  RCGCTIMER |= 0x1; //Enable Timer 0 and Timer 1
  
  TIMER0_CTL = 0x0; //Verify Timer is not running
  
  TIMER0_CFG = 0x0; //Timer is 32 bit
  
  TIMER0_TAMR = 0x1; // This is the one-shot that it only starts the timer again when it is reset through this method
  
  TIMER0_TAILR = 0x00F42400 * 2; //Timer set to 16000000* 2 = 2 seconds
  
  //Interrupts
  NVIC_EN0 |= 0x80000; 
  TIMER0_IMR = 0x1;
  //TIMER0_CTL |= 0x01; //Enable Timer
  
  TIMER0_ICR = 0x01;
}

//This method turns Timer0 on
void Timer0_ON() {
  TIMER0_CTL = 0x1;
}

//This method turns Timer1 on
void Timer1_ON() {
  TIMER1_CTL = 0x1;
}

