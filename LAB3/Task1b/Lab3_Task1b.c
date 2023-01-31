/**
 * EE/CSE 474: Lab3 Task1b main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/2/2022
 * This main program prints the microcontroller's temperature to the output terminal. The computer - microcontroller communication
 * is done through UART0 virtual pins.
 */


#include <stdint.h>
#include "Lab3_Inits.h"
#include <stdio.h>

#define SW1 0x1
#define SW2 0x2

// STEP 0b: Include your header file here
#include "lab3.h"

//Global ADC Value that is updated in the handler.
uint32_t ADC_value;


int main(void) {
  // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float resistance;      //Initialize resistance value
  
  float temp;            //Initialize the temp value and pointer.
  float *ptmp = &temp;   
   
  
    
  while(1) {
    // STEP 5: Change the pattern of LEDs based on the resistance.
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    //resistance = ((ADC_value / 4095.0) * 10.0);
    // 5.2: Change the pattern of LEDs based on the resistance
    
    *ptmp = 147.5 - ((75 * 3.3 * ADC_value) / 4096.0);  //VREFP = 3.3V, VREFN = 0V in equation provided in Datasheet. Set temp value here.
     printf("%f degrees celsius\n", *ptmp);             //Print the temp value to output
     
     //Logic to slow down system clock. Cool down system temperature.
    if (!(GPIODATA_J & 0x1)) {
      freq = PRESET3;
      PLL_Init(freq);
    } 
    
    //Logic to speed up system clock. Heat up system temperature.
    if (!(GPIODATA_J & 0x2)) {
      freq = PRESET1;
      PLL_Init(freq);
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

