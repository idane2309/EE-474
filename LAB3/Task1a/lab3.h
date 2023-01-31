/* 
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022 
 * Header File
 */ 

#ifndef lab3_h  //Header Guard
#define lab3_h

// To set up and change the data for GPIO: LEDs and switches
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))       //Set up RCGCPIO

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))      //Sets up and assigns GPIOs for Port F LED 3 and LED 4
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))
#define GPIOAFSEL_F (*((volatile uint32_t *)0x4005D420))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))      //Sets up and assigns GPIOs for Port N LED 1 and LED 2
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))
#define GPIOAFSEL_N (*((volatile uint32_t *)0x40064420))


//Timer 0 set Up and configurations
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604)) //Enables Timer0
#define TIMER0_CTL (*((volatile uint32_t *)0x4003000C))   
#define TIMER0_CFG (*((volatile uint32_t *)0x40030000))  
#define TIMER0_TAMR (*((volatile uint32_t *)0x40030004))     
#define TIMER0_TAILR (*((volatile uint32_t *)0x40030028))   
#define TIMER0_RIS (*((volatile uint32_t *)0x4003001C))
#define TIMER0_ICR (*((volatile uint32_t *)0x40030024))
#define TIMER0_IMR (*((volatile uint32_t *)0x40030018))  //IMR for interrupt. Offset = 0x018

#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))  //NVIC EN0 interrupt offset = 0x100, Base = 0xE000E000 PORTF and Timer 0 

#define NVIC_EN2 (*((volatile uint32_t *)0xE000E108))  //NVIC EN0 interrupt offset = 0x108, Base = 0xE000E000 PORTN

#define TIMER0_ADCEV (*((volatile uint32_t *)0x40030070)) //FOR AD0, SS3, TIMER 0

//ADC0 PORT E 
#define GPIOAFSEL_ADC (*((volatile uint32_t *)0x4005C420)) //PORT E ADC PIN PE3: BASE: 0x40005C, OFFSET = 0x420
#define GPIODEN_ADC (*((volatile uint32_t *)0x4005C51C)) //PORT E ADC PIN PE3: BASE: 0x40005C, OFFSET = 0x51C
#define GPIOAMSEL_ADC (*((volatile uint32_t *)0x4005C528)) //PORT E ADC PIN PE3: BASE: 0x40005C, OFFSET = 0x528
#define GPIOADIR_ADC (*((volatile uint32_t *)0x4005C400))  //PORT E ADC PIN PE3: BASE: 0x40005C, OFFSET = 0x400
#define GPIOADATA_ADC (*((volatile uint32_t *)0x4005C3FC)  //PORT E ADC PIN PE3: BASE: 0x40005C, OFFSET = 0x3FC
   
//For ADC PE3
#define RCGCADC (*((volatile uint32_t *)0x400FE638)) //Enabler for clocks
#define ADCCC (*((volatile uint32_t *)0x40038FC8)) // FOR ADC0 Clock Base = 0x4003899 Offset = 0xFC8
#define ADCACTSS (*((volatile uint32_t *)0x40038000)) //Enabler for Sample Sequences.  Base 0x40038 for AD0. Offset = 0x000
#define ADCEMUX (*((volatile uint32_t *)0x40038014)) //Enabler for trigger event. Base 0x40038 for AD0. Offset = 0x014

//For Sample Sequence 3
#define ADCSSMUX3 (*((volatile uint32_t *)0x400380A0)) //FOR AD0, SS3
#define ADCSSEMUX3 (*((volatile uint32_t *)0x400380B8)) //FOR AD0, SS3
#define ADCSSCTL3 (*((volatile uint32_t *)0x400380A4)) //FOR AD0, SS3
#define ADCIM (*((volatile uint32_t *)0x40038008)) //FOR AD0, SS3
#define NVIC_ADCSS3 (*((volatile uint32_t *)0xE000E100)) //FOR AD0, SS3
#define ADCPSSI (*((volatile uint32_t *)0x40038028)) //FOR AD0, SS3 TO ENABLE
#define ADCSSFIFO3 (*((volatile uint32_t *)0x400380A8)) //FOR AD0, SS3
#define ADCISC (*((volatile uint32_t *)0x4003800C)) //FOR AD0, SS3

//Forward Declarations
void PLL_Init(enum frequency);       
void LED_Init(void);            
void ADCReadPot_Init(void);     
void TimerADCTriger_Init(void);

#endif //lab3.h header file