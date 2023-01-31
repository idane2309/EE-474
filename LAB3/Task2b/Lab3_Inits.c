/**
 * EE/CSE 474: Lab3 drivers starter code
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
 * Initialization File to initialize methods and setup.
 */

#include "PLL_Header.h"
#include "Lab3_Inits.h"


#define SW1 0x1
#define SW2 0x2

//Forward Declarations
void Delay(unsigned long counter);

// STEP 0a: Include your header file here
#include "lab3.h"

//PLL Initialization
int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    MOSCCTL &= ~(0x4);                      // Power up MOSC
    MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    PLLFREQ0 |= 0x60;                       // Set MINT field
    PLLFREQ1 |= 0x4;                        // Set N field

    MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

//LED and Switches Initialization
void LED_Init(void) {
  // STEP 1: Initialize the 4 on board LEDs by initializing the corresponding
  // GPIO pins.
 RCGCGPIO |= 0x100; // Enable PortJ

 /*GPIODIR_F = 0x11; // Set PF0 and PF4 to output for LEDs 3 and 4
 GPIODEN_F = 0x11; // Set PF0 and PF4 to digital port for LEDs 3 and 4
 
 GPIODIR_N = 0x3; // Set PN0 and PN1 to output for LEDs 1 and 2
 GPIODEN_N = 0x3; // Set PN0 and PN1 to digital port for LEDS 1 and 2*/
 
  GPIODIR_J = 0x0; // Set PJ0 and PJ1 to input for Switches 1 and 2
  GPIODEN_J = 0x3; //Set PJ0 and PJ1 to digital port for Switches 1 and 2
  GPIOPUR_J = 0x3;  // Attach pull-up resistors to pins PJ0 and PJ1 
  
 
}

//ADC Initialization
void ADCReadPot_Init(void) {
  // STEP 2: Initialize ADC0 SS3.
  // 2.1: Enable the ADC0 clock
  RCGCADC |= 0x1; //Enable and provide a clock for ADC module 0 in run mode. 
  // 2.2: Delay for RCGCADC (Refer to page 1073)
  Delay(1);
  // 2.3: Power up the PLL (if not already)
  PLLFREQ0 |= 0x00800000; // we did this for you
  // 2.4: Wait for the PLL to lock
  while (PLLSTAT != 0x1); // we did this for you
  // 2.5: Configure ADCCC to use the clock source defined by ALTCLKCFG
  ADCCC |= 0x1; //Alternate clock source config register in system control module. 
  // 2.6: Enable clock to the appropriate GPIO Modules (Hint: Table 15-1)
  RCGCGPIO |= 0x10; //ENABLE CLOCK TO PORTE
  // 2.7: Delay for RCGCGPIO
  Delay(1);
  // 2.8: Set the GPIOAFSEL bits for the ADC input pins
  GPIOAFSEL_ADC |= 0x8; //Set GPIOAFSEL for pin PE3. 
  // 2.9: Clear the GPIODEN bits for the ADC input pins
  GPIODEN_ADC &= ~0x8;  //Clear GPIODEN for pin PE3.
  // 2.10: Disable the analog isolation circuit for ADC input pins (GPIOAMSEL)
  GPIOAMSEL_ADC &= ~0x8; //Disable Analog isolation for pin PE3.
  // 2.11: Disable sample sequencer 3 (SS3)
  ADCACTSS &= ~0x8; 
  // 2.12: Select timer as the trigger for SS3
  ADCEMUX |= 0x5000; //15:12 put in Bit 5 value. 
  // 2.13: Select the analog input channel for SS3 (Hint: Table 15-1)
  ADCSSMUX3 &= 0x0;
  // 2.14: Configure ADCSSCTL3 register
  ADCSSCTL3 |= 0xE; //Bit 3 Configured to 1 here for the temperature sensor for bit 3. We want bit 2 and 1 to be set to 0. Bit 0 set to 0. 
  // 2.15: Set the SS3 interrupt mask
  ADCIM |= 0x8; //SS3 MASK
  // 2.16: Set the corresponding bit for ADC0 SS3 in NVIC
  NVIC_ADCSS3 |= 0x20000; //For interrupt bit 17
  // 2.17: Enable ADC0 SS3
  GPIODEN_ADC |= 0x8;
  ADCACTSS |= 0x8;
  
  //Temperature sensor hold register set to 16 adc clocks.
  ADCSSTSH3 |= 0x4;

}

//TIMER0A ADC0 Intialization
void TimerADCTriger_Init(void) {
  // STEP 3: Initialize Timer0A to trigger ADC0 at 1 HZ.
  // Hint: Refer to section 13.3.7 of the datasheet
  
  //Configure Timer
  RCGCTIMER = 0x01; //Enable Timer 0
  Delay(1); //Delay for timer
  TIMER0_CTL = 0x0; //Verify Timer is not running
  TIMER0_CFG = 0x0; //Timer is 32 bit
  TIMER0_TAMR = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  TIMER0_TAILR = 0x00F42400; //Timer set to 16000000. 1 Hz
  
  //Interrupts
  //NVIC_EN0 |= 0x80000; 
  //TIMER0_IMR = 0x1;
  TIMER0_ADCEV |= 0x1; //Timer ADC Trigger Event Enable  
  TIMER0_ICR = 0x1;
  TIMER0_CTL |= 0x21; //Enable Timer ADC trigger BIT 5
           
  
}

// NEXT STEP: Go to Lab3_Task1a.c and finish implementing ADC0SS3_Handler

//Delay method
void Delay(unsigned long counter) {
  unsigned long i = 0;
  for (i = 0; i < counter*1000; i++);
}

//UART4 Initialization
void UART_initialize(void) {
  RCGCUART |= 0x10;  //Enable UART4
  RCGCGPIO |= 0x200;   //Enable Port K
  Delay(1);
  GPIODEN_K = 0x3;  //SET PINS PK0/PK1 to Digital Output
  GPIOAFSEL_K = 0x3; //PK0/PK1 ALTERNATE FUNCTION
  GPIOPCTL_K = 0x11;  //SET PORT MUX CONTROL FOR BOTH PINS PK1/PK0 FOR UART4
  Delay(1);
  UARTCTL4 = 0x0;   //DISABLE UART4 FOR CONFIGURATION
  UARTIBRD4 = 390;   //UART BAUD RATE = (F / 16 x BAUD DIVISOR) --> 9600 = (60 / 16 x BAUD DIVISOR) --> BAUD DIVISOR = 104.667 --> IBRD = 390
  UARTFBRG4 = 40;    //FRACTION PART OF BAUD GENERATOR:(0.625 x 64) + 0.5 = 40
  UARTCC4 = 0x0;    //SELECT THE SYSTEM CLOCK FOR UART
  UARTLCRH4 = 0x60; //SET WORD LENGTH 8 BITS. IF WANT TO ENABLE FIFO THEN SET TO 0x70 to expand to 16 byte buffer,=.
  UARTCTL4 = 0x301; //ENABLE TXE, RXE AND UARTEN IN CTL 
}
