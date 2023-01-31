/**
 * EE/CSE 474: Lab3 Task1a main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/2/2022
 */

// NOTE: This is the main function for Task 1a. You should start by
// implementing Lab3_Driver.c (STEP 0a, 1-3) and then come back to finish
// the ISR and the main function (STEP 0b, 4-5).

#include <stdint.h>
#include "Lab3_Inits.h"

#define SW1 0x1
#define SW2 0x2

// STEP 0b: Include your header file here
#include "lab3.h"

//Our Global ADC Value updated through the ADC handler
uint32_t ADC_value;


int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float resistance;      //Initialize resistance value
  GPIODATA_F = 0x0;
  GPIODATA_N = 0x0;
  while(1) {
    // STEP 5: Change the pattern of LEDs based on the resistance.
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    resistance = ((ADC_value / 4095.0) * 10.0);
    // 5.2: Change the pattern of LEDs based on the resistance
    if (resistance < 2.5) {
      GPIODATA_F = 0x1;
      GPIODATA_N = 0x0;
    } else if (resistance < 5.0) {
      GPIODATA_F = 0x11;
      GPIODATA_N = 0x0;
    } else if (resistance < 7.5) {
      GPIODATA_F = 0x11;
      GPIODATA_N = 0x1;
    } else {
      GPIODATA_F = 0x11;
      GPIODATA_N = 0x3;

    }
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

