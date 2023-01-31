/**
 * EE/CSE 474: Lab3 Task2a main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
 * This main file is used to communicate with the microcontoller through UART0 and communicate
 * the current temperature value of the system to the computer through PUTTY.
 */

// NOTE: This is the main function for Task 2a. You should start by
// implementing Lab3_Driver.c (STEP 0a, 1-3) and then come back to finish
// the ISR and the main function (STEP 0b, 4-5).

#include <stdint.h>
#include "Lab3_Inits.h"
#include <stdio.h>

#define SW1 0x1
#define SW2 0x2

// STEP 0b: Include your header file here
#include "lab3.h"

//Initialize Global value ADC
uint32_t ADC_value;

int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  UART_initialize();     //Initialize UART0
  
  float temp;            //Tenp value initialization
     
  while(1) {
    // STEP 5: Change the pattern of LEDs based on the resistance.
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    //resistance = ((ADC_value / 4095.0) * 10.0);
    // 5.2: Change the pattern of LEDs based on the resistance
    
    temp = 147.5 - ((75 * 3.3 * ADC_value) / 4096.0);  //VREFP = 3.3V, VREFN = 0V. Temp calculation based on equation provided by datasheet.
     
    //Here we create a char array containing the string converted temp float value. We then print this into our putty terminal
    //through the transmit method.
    char strTmp[20];
    sprintf(strTmp, "%f", temp);
    stringConvert("Temp in C:");
    stringConvert(strTmp);
    stringConvert("\r\n");
    Delay(2000);
    

  }
  
  
  return 0;
}

void ADC0SS3_Handler(void) {
  // STEP 4: Implement the ADC ISR.
  // 4.1: Clear the ADC0 interrupt flag
    ADCISC |= 0x8;
    TIMER0_ICR = 0x1;
  // 4.2: Save the ADC value to global variable ADC_value
     ADC_value = ADCSSFIFO3; 
     
     
}

//Method to transmit data parameter input from microcontroller UART0
void UART0_Transmitter(char data) {
  while (UARTFR & 0x8) {};
  UARTDR = data;
}

//Converts inputted string to char array that is inputted into the transmit method to be transmitted.
void stringConvert(char *str) {
  while(*str) {
    UART0_Transmitter(*(str++));
  }
}


