 /**
 * EE/CSE 474: Lab3 Task1a main function
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
 * This main program takes a character input from the computer through the PUTTY terminal; The microcontroller recieves this character input via a
 * bluetooth module and then transmits the character back to the computer PUTTY terminal. The character then shows up on the putty terminal. 
 */



#include <stdint.h>
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include <stdio.h>


// STEP 0b: Include your header file here
#include "tm4c1294ncpdt.h"
   
//Global ADC Value that is updated in the handler.
uint32_t ADC_value;
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};


//Forward Declarations
void Delay(unsigned long counter);
int PLL_Init(enum frequency freq);
void LED_Init(void);
void ADCReadPot_Init(void);
void TimerADCTriger_Init(void);


int main(void) {
   // Select system clock frequency preset
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz
  LED_Init();            // Initialize the 4 onboard LEDs (GPIO)
  ADCReadPot_Init();     // Initialize ADC0 to read from the potentiometer
  TimerADCTriger_Init(); // Initialize Timer0A to trigger ADC0
  float resistance;      //Initialize resistance value
  
  float tempC;            //Initialize the temp value and pointer.
  float tempF;
   
  
  LCD_Init(); 
  
  
  
  while(1) {
    // STEP 5: Change the pattern of LEDs based on the resistance.
    // 5.1: Convert ADC_value to resistance in kilo-ohm
    //resistance = ((ADC_value / 4095.0) * 10.0);
    // 5.2: Change the pattern of LEDs based on the resistance
    
    tempC = 147.5 - ((75 * 3.3 * ADC_value) / 4096.0);  //VREFP = 3.3V, VREFN = 0V in equation provided in Datasheet. Set temp value here.
    tempF = (tempC * 1.8) + 32;
    printf("The current temperatue is %f C\n", tempC); 
    
    LCD_Printf("The current temperature is ");
    LCD_PrintFloat(tempC);
    LCD_Printf(" C, ");
    LCD_PrintFloat(tempF);
    LCD_Printf(" F.\n");
    LCD_Printf("The current clock frequency is %d MHz    \n", freq);
  

    // Set text cursor to top left of screen
    LCD_SetCursor(0, 0);
    
     
     //Logic to slow down system clock. Cool down system temperature.
    if (!(GPIO_PORTJ_DATA_R & 0x1)) {
      freq = PRESET1;
      PLL_Init(freq);
    } 
    
    //Logic to speed up system clock. Heat up system temperature.
    if (!(GPIO_PORTJ_DATA_R & 0x2)) {
      freq = PRESET3;
      PLL_Init(freq);
    }
    
  }
  
  
  return 0;
}

void ADC0SS3_Handler(void) {
  // STEP 4: Implement the ADC ISR.
  // 4.1: Clear the ADC0 interrupt flag
    ADC0_ISC_R |= 0x8;
    TIMER0_ICR_R = 0x1;
  // 4.2: Save the ADC value to global variable ADC_value
     ADC_value = ADC0_SSFIFO3_R;
}




int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    SYSCTL_MOSCCTL_R &= ~(0x4);                      // Power up MOSC
    SYSCTL_MOSCCTL_R &= ~(0x8);                      // Enable MOSC
    while ((SYSCTL_RIS_R & 0x100) == 0) {};          // Wait for MOSC to be ready
    SYSCTL_RSCLKCFG_R |= (0x3 << 20);                // Select MOSC as system clock source
    SYSCTL_RSCLKCFG_R |= (0x3 << 24);                // Select MOSC as PLL clock source

    SYSCTL_PLLFREQ0_R |= 0x60;                       // Set MINT field
    SYSCTL_PLLFREQ1_R |= 0x4;                        // Set N field

    SYSCTL_MEMTIM0_R &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    SYSCTL_MEMTIM0_R &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    SYSCTL_MEMTIM0_R &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    SYSCTL_RSCLKCFG_R &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            SYSCTL_MEMTIM0_R |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            SYSCTL_MEMTIM0_R |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            SYSCTL_RSCLKCFG_R |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            SYSCTL_RSCLKCFG_R &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            SYSCTL_MEMTIM0_R |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            SYSCTL_MEMTIM0_R |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            SYSCTL_RSCLKCFG_R |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            SYSCTL_RSCLKCFG_R &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            SYSCTL_MEMTIM0_R |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            SYSCTL_RSCLKCFG_R |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            SYSCTL_RSCLKCFG_R &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    SYSCTL_RSCLKCFG_R |= (0x1 << 30);                // Enable new PLL settings
    SYSCTL_PLLFREQ0_R |= (0x1 << 23);                // Power up PLL
    while ((SYSCTL_PLLSTAT_R & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    SYSCTL_RSCLKCFG_R |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

//Method to initialize the ports and pins for switches.
void LED_Init(void) {
  // STEP 1: Initialize the 4 on board LEDs by initializing the corresponding
  // GPIO pins.
 SYSCTL_RCGCGPIO_R |= 0x100; // Enable PortJ GPIOs

  GPIO_PORTJ_DIR_R = 0x0; // Set PJ0 and PJ1 to input for Switches 1 and 2
  GPIO_PORTJ_DEN_R = 0x3; //Set PJ0 and PJ1 to digital port for Switches 1 and 2
  GPIO_PORTJ_PUR_R = 0x3;  // Attach pull-up resistors to pins PJ0 and PJ1 
  
 
}

//Method to initialize the ADC
void ADCReadPot_Init(void) {
  // STEP 2: Initialize ADC0 SS3.
  // 2.1: Enable the ADC0 clock
  SYSCTL_RCGCADC_R |= 0x1; //Enable and provide a clock for ADC module 0 in run mode. 
  // 2.2: Delay for RCGCADC (Refer to page 1073)
  Delay(1);
  // 2.3: Power up the PLL (if not already)
  SYSCTL_PLLFREQ0_R |= 0x00800000; // we did this for you
  // 2.4: Wait for the PLL to lock
  while (SYSCTL_PLLSTAT_R != 0x1); // we did this for you
  // 2.5: Configure ADCCC to use the clock source defined by ALTCLKCFG
  ADC0_CC_R |= 0x1; //Alternate clock source config register in system control module. 
  // 2.6: Enable clock to the appropriate GPIO Modules (Hint: Table 15-1)
  SYSCTL_RCGCGPIO_R |= 0x10; //ENABLE CLOCK TO PORTE
  // 2.7: Delay for RCGCGPIO
  Delay(1);
  // 2.8: Set the GPIOAFSEL bits for the ADC input pins
  GPIO_PORTE_AFSEL_R |= 0x8; //Set GPIOAFSEL for pin PE3. 
  // 2.9: Clear the GPIODEN bits for the ADC input pins
  GPIO_PORTE_DEN_R &= ~0x8;  //Clear GPIODEN for pin PE3.
  // 2.10: Disable the analog isolation circuit for ADC input pins (GPIOAMSEL)
  GPIO_PORTE_AMSEL_R &= ~0x8; //Disable Analog isolation for pin PE3.
  // 2.11: Disable sample sequencer 3 (SS3)
  ADC0_ACTSS_R &= ~0x8; 
  // 2.12: Select timer as the trigger for SS3
  ADC0_EMUX_R |= 0x5000; //15:12 put in Bit 5 value. 
  // 2.13: Select the analog input channel for SS3 (Hint: Table 15-1)
  ADC0_SSMUX3_R &= 0x0;
  // 2.14: Configure ADCSSCTL3 register
  ADC0_SSCTL3_R |= 0xE; //Bit 3 Configured to 1 here for the temperature sensor for bit 3. We want bit 2 and 1 to be set to 0. Bit 0 set to 0. 
  // 2.15: Set the SS3 interrupt mask
  ADC0_IM_R |= 0x8; //SS3 MASK
  // 2.16: Set the corresponding bit for ADC0 SS3 in NVIC
  NVIC_EN0_R |= 0x20000; //For interrupt bit 17
  // 2.17: Enable ADC0 SS3
  GPIO_PORTE_DEN_R |= 0x8;
  ADC0_ACTSS_R |= 0x8;
  
  //Temperature sensor hold register set to 16 adc clocks.
  ADC0_SSTSH3_R |= 0x4;

}

//Method to initialize the TIMER0A trigger ADC0.
void TimerADCTriger_Init(void) {
  // STEP 3: Initialize Timer0A to trigger ADC0 at 1 HZ.
  // Hint: Refer to section 13.3.7 of the datasheet
  
  //Configure Timer
  SYSCTL_RCGCTIMER_R = 0x01; //Enable Timer 0
  Delay(1); //Delay for timer
  TIMER0_CTL_R = 0x0; //Verify Timer is not running
  TIMER0_CFG_R = 0x0; //Timer is 32 bit
  TIMER0_TAMR_R = 0x2; //Timer is periodic (TAMR BIT) and counts down (TACDIR
  TIMER0_TAILR_R = 0x00F42400; //Timer set to 16000000. 1 Hz
  
  //Interrupts
  //NVIC_EN0 |= 0x80000; 
  //TIMER0_IMR = 0x1;
  TIMER0_ADCEV_R |= 0x1; //Timer ADC Trigger Event Enable  
  TIMER0_ICR_R = 0x1;
  TIMER0_CTL_R |= 0x21; //Enable Timer ADC trigger BIT 5
           
  
}

// NEXT STEP: Go to Lab3_Task1a.c and finish implementing ADC0SS3_Handler

//Delay method
void Delay(unsigned long counter) {
  unsigned long i = 0;
  for (i = 0; i < counter*1000; i++);
}


