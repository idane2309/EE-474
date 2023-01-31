/**
 * EE/CSE 474: Lab3 Task1a main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
 * This main program takes a character input from the computer through the PUTTY terminal; The microcontroller recieves this character input via a
 * bluetooth module and then transmits the character back to the computer PUTTY terminal. The character then shows up on the putty terminal. 
 */



#include <stdint.h>
#include "Lab3_Inits.h"
#include <stdio.h>

#define SW1 0x1
#define SW2 0x2

// STEP 0b: Include your header file here
#include "lab3.h"


//Global ADC value updated by the handler
uint32_t ADC_value;


int main(void) {
 // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  UART_initialize();     //Initialize UART4 and Pins PK1/PK0
  
   
  while(1) {
    
    //Logic to recieve and transmit data from microcontroller. Checks if the UART Reciever FIFO is full and then 
    //reads and transmits. UARTDR4 = UARTDR4 works because there is unintentionally a delay in the microcontroller 
    //in between tasks that provides just enough of a window for it to read and then transmit through this line.
    if (UARTFR4 & 0x40) {
      //char data = UARTDR4;
      //UARTDR4 = data;
      UARTDR4 = UARTDR4;
    }
    
    
  }
  
  
  return 0;
}

//ADC Handler method
void ADC0SS3_Handler(void) {
  // STEP 4: Implement the ADC ISR.
  // 4.1: Clear the ADC0 interrupt flag
    ADCISC |= 0x8;
    TIMER0_ICR = 0x1;
  // 4.2: Save the ADC value to global variable ADC_value
     ADC_value = ADCSSFIFO3; 
     
     
}





