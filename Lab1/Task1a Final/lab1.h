/* 
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 10/12/2022 
 */ 

#ifndef lab1.h  //Header Guard
#define lab1.h

// To set up and change the data for GPIO: LEDs and switches
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))       //Set up RCGCPIO

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))      //Sets up and assigns GPIOs for Port F LED 3 and LED 4
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))      //Sets up and assigns GPIOs for Port N LED 1 and LED 2
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))



#endif //lab1.h header file