/* 
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 11/17/2022 
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

//PORT J
#define GPIODIR_J (*((volatile uint32_t *)0x40060400))  //Sets up and assigns GPIOs for Port J Switches 1 and 2
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC))
#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))
//Port J Interrupts
#define GPIO_PORTJ_IS (*((volatile uint32_t *)0x40060404))
#define  GPIO_PORTJ_IBE (*((volatile uint32_t *)0x40060408))
#define  GPIO_PORTJ_IEV (*((volatile uint32_t *)0x4006040C))
#define  GPIO_PORTJ_ICR (*((volatile uint32_t *)0x4006041C))
#define  GPIO_PORTJ_IM (*((volatile uint32_t *)0x40060410))
#define  GPIO_PORTJ_PRI (*((volatile uint32_t *)0xE000E430)) //Priority Register for Interrupt 
#define  GPIO_PORTJ_MIS (*((volatile uint32_t *)0x40060418)) //MIS
#define NVIC_EN1 (*((volatile uint32_t *)0xE000E104))  //NVIC EN0 interrupt offset = 0x104, Base = 0xE000E000 PORTJ

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
#define GPIOADIR_ADC (*((volatile uint32_t *)0x4005C400))
#define GPIOADATA_ADC (*((volatile uint32_t *)0x4005C3FC)      
   
//For ADC PE3
#define RCGCADC (*((volatile uint32_t *)0x400FE638)) //Enabler for clocks
#define ADCCC (*((volatile uint32_t *)0x40038FC8)) // FOR ADC0 Clock Base = 0x4003899 Offset = 0xFC8
#define ADCACTSS (*((volatile uint32_t *)0x40038000)) //Enabler for Sample Sequences.  Base 0x40038 for AD0. Offset = 0x000
#define ADCEMUX (*((volatile uint32_t *)0x40038014)) //Enabler for trigger event. Base 0x40038 for AD0. Offset = 0x014
#define ADCSSTSH3 (*((volatile uint32_t *)0x400380BC)) //REGISTER FOR TEMPETURE SENSOR HOLD TO 16 ADC CLOCKS

//For Sample Sequence 3
#define ADCSSMUX3 (*((volatile uint32_t *)0x400380A0)) //FOR AD0, SS3
#define ADCSSEMUX3 (*((volatile uint32_t *)0x400380B8)) //FOR AD0, SS3
#define ADCSSCTL3 (*((volatile uint32_t *)0x400380A4)) //FOR AD0, SS3
#define ADCIM (*((volatile uint32_t *)0x40038008)) //FOR AD0, SS3
#define NVIC_ADCSS3 (*((volatile uint32_t *)0xE000E100)) //FOR AD0, SS3
#define ADCPSSI (*((volatile uint32_t *)0x40038028)) //FOR AD0, SS3 TO ENABLE
#define ADCSSFIFO3 (*((volatile uint32_t *)0x400380A8)) //FOR AD0, SS3
#define ADCISC (*((volatile uint32_t *)0x4003800C)) //FOR AD0, SS3

//UART0 SETUP PINS PA0/PA1
#define RCGCUART (*((volatile uint32_t *)0x400FE618))
#define UARTCTL (*((volatile uint32_t *)0x4000C030))
#define UARTFBRG (*((volatile uint32_t *)0x4000C028))
#define UARTIBRD (*((volatile uint32_t *)0x4000C024))
#define UARTCC (*((volatile uint32_t *)0x4000CFC8))
#define UARTLCRH (*((volatile uint32_t *)0x4000C02C))
#define UARTDR (*((volatile uint32_t *)0x4000C000))
#define UARTFR (*((volatile uint32_t *)0x4000C018))
#define GPIOPCTL_A (*((volatile uint32_t *)0x4005852C))
#define GPIODIR_A (*((volatile uint32_t *)0x40058400))
#define GPIODEN_A (*((volatile uint32_t *)0x4005851C))
#define GPIODATA_A (*((volatile uint32_t *)0x400583FC))
#define GPIOAFSEL_A (*((volatile uint32_t *)0x40058420))

//Forward Declarations
void PLL_Init(enum frequency);       
void LED_Init(void);            
void ADCReadPot_Init(void);     
void TimerADCTriger_Init(void);
void UART_initialize(void);


#endif //lab3.h header file