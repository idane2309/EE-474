// This is the starter code for the Lab 4 Task 2B.
// Designed for ECE/CSE 474.
//
// Please read through the entire file and make sure
// you understand your tasks, the structure of the code,
// and the hints you are given (although you are not
// required to follow them)
//
// The only requirement is to use the three tasks specified
// in the main function. Feel free to add/delete the code we
// provided, though it is recommended to follow the hints.
//
// Here's the documentation page for FreeRTOS.
// You may want to look at it when doing this assignment.
// https://www.freertos.org/FreeRTOS-quick-start-guide.html

/*
 * Ishan Dane & Anael Aguayo-Chong
 * 1937153 & 2068579
 * 12/2/2022
 * Main Program for Lab 4 Task 2b
 * This main program uses RTOS to display a working Traffic light system on an LCD screen with virtual buttons.
 * When the system is turned on by the on/off pushbutton, the system starts at the red light and then alternates between the green light and the red stop
 * light. If the pedestrian pushbutton is pressed on the green light, the green light then transitions to a yellow warning light then to a red light. 
 * If the pedestrian pushbutton is pressed on the red light, nothing happens. Each button press has to be held for 2 seconds before it is acknowledged by 
 * the system. 
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "Task2b_h.h"
#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "PLL_Header.h"
// Include the LCD driver's header file, the PLL driver's header file,
// and your own header file here.

// After skimming through the entire file,
// you might want to add some FSM states here.
enum TRAFF_States {idle, stop, warn, go} TRAFF_State;  //Create states with their assigned names. 
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12}; //Names for PLL Frequency. PLL frequency enum type declaration.

// Hint:
//
// The following global variables are flags you may want to use
//int pedestrian_status = 0;
int increase_status = 0;
int decrease_status = 0;
int onoff_status = 0;


// Below are function prototypes for our RTOS task.
// You should not change the function declerations.
// However, you are allowed to add more helper functions
// as needed.
void PWM_Init();
void LED_Init();
int PLL_Init(enum frequency freq);
// Task function that checks the state of the virtual pedestrian button.
// Keeps track of how many seconds the pedestrian button has been pressed.
// Once the user has pressed the virtual pedestrian button for 2 seconds,
// it will set the global flag indicating the virtual pedestrian button
// has been pressed.
//void Pedestrian(void *p);
void Increase(void *p);

// Task function that checks the state of the virtual onoff button.
// Keeps track of how many seconds the onoff button has been pressed.
// Once the user has pressed the onoff button for 2 seconds,
// it will set the global flag indicating the onoff button has been
// pressed
void StartStop(void *p);

// Task function that represents your Finite State Machine.
// Keeps track of how many seconds the virtual traffic light has been
// lighting on. It will update the state of the traffic light system
// every 5 seconds or once there's virtual button input.
//void Control(void *p);
void Decrease(void *p);

// Helper function that represents your FSM.
// You are allowed to change this function decleration.
//
// Handles the traffic light state transition.
void FSM(void);


// Do not modify this function.
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName) {
  // This function can not return, so loop forever.  Interrupts are disabled
  // on entry to this function, so no processor interrupts will interrupt
  // this loop.
  while (1) {}
}


// Initialize FreeRTOS and start the tasks.
int main(void) {
  // STEP 1
  LCD_Init();                   //LCD Screen Initialization
  Touch_Init();                 //Touch Initialization
  
  // This is the beginning of the main function,
  // Initialize your system by initialize the display and touch
  // functionalities of your SSD2119 touch display assembly. You may
  // also want to initialize (draw) your virtual buttons here.
  //LCD_DrawCircle(50,60,45, Color4[10]);
  LCD_DrawCircle(160,60,45, Color4[14]);
  LCD_SetCursor(140, 60);
  LCD_Printf("ON | OFF");
  //LCD_DrawCircle(270,60,45, Color4[12]);
    
  LCD_DrawCircle(90,180,45,Color4[15]); 
  LCD_SetCursor(70,180);
  LCD_Printf("DECREASE");
  LCD_SetCursor(210,180);
  LCD_Printf("INCREASE");
  LCD_DrawCircle(240,180,45, Color4[15]);
  // Moreover, initialize the PLL to set the system clock to 60 MHz.
  enum frequency freq = PRESET2; // 60 MHz
  PLL_Init(freq);        // Set system clock frequency to 60 MHz


  // STEP 2
  //
  // The code below creates three tasks.
  // Your task here is to assign priorities to the tasks.
  //
  // Think about which task(s) should be given the highest
  // priority and which should be given lower. It is possible to assign
  // the same priority to all the tasks, however.
  //
  // Priorities are in range: [0, configMAX_PRIORITIES - 1], where
  // configMAX_PRIORITIES - 1 corresponds to the highest priority and
  // 0 corresponds to the lowest priority.
  // You can find configMAX_PRIORITIES defined in the file called FreeRTOSConfig.h
  // under the freertos_demo directory.
  //
  // You should not create more tasks. However, you are allowed to add as many
  // helper functions as you want.
  // xTaskCreate(Function Name,
  //             Descriptive Task Name,
  //             Stack Depth,
  //             Task Function Parameter,
  //             Priority,
  //             Task Handle);
  xTaskCreate(StartStop, (const char *)"StartStopButton", 1024, NULL, 3, NULL);
  //xTaskCreate(Pedestrian, (const char *)"PedestrianButton", 1024, NULL, 2, NULL);
  xTaskCreate(Increase, (const char *)"IncreaseButton", 1024, NULL, 2, NULL);
  xTaskCreate(Decrease, (const char *)"DecreaseButton", 1024, NULL, 2, NULL);
  //xTaskCreate(Control, (const char *)"Control FSM", 1024, NULL, 1, NULL);

  // Start the scheduler. This should not return.
  // The scheduler will do the scheduling and switch between
  // different tasks for you.
  // Refer to the lecture slides and RTOS documentation
  // page for more details about task scheduling and context switching.
  //
  // One important concept for context switching and task scheduling
  // is preemption. Think about what the terminology preemptive and
  // non-preemptive mean.
  //
  // Hint: Non-preemptive scheduling will allow other tasks to be scheduled
  // after the current task has entered the "waiting state". That is, in our context,
  // the current task will keep running until it reaches vTaskDelay
  // (you'll see this in task functions).
  //
  // You can find in FreeRTOSConfig.h the setting of preemption
  // for the RTOS. Feel free to change this setting as needed.
  vTaskStartScheduler();

  // In case the scheduler returns for some reason, loop forever.
  while(1) {}
}

void StartStop(void *p) {
  // Hint:
  //
  // Static variable will only be decleared once and
  // will retain its last assigned value until the entire
  // program exits
  static int curr_onoff_tick_time = 0;
  static int prev_onoff_tick_time = 0;

  // xTaskGetTickCount() will return the count of ticks
  // since the Task Scheduler started (i.e. your program starts executing).
  //
  // The tick rate (configTICK_RATE_HZ) is defined in FreeRTOSConfig.h,
  // the default rate is 1000 (1KHz), so one tick is equivalent to 1 ms
  //
  // It is similar to the timer we used before, but you'll
  // need to calculate the time elapsed by taking the difference
  // between curr_tick and prev_tick.
  curr_onoff_tick_time = xTaskGetTickCount();

  // STEP 3
  //
  // Complete the implementation of this task function.
  // It checks whether the virtual StartStop button has been
  // pressed for 2 seconds and needs to set the global flags accordingly
  //
  // Task function should never return so is typically
  // implemented as an infinite loop
  //
  // The vTaskDelay(n) will cause the task to enter Blocked state
  // for n system clock ticks. i.e. the task is unblocked
  // after n systicks and will enter Ready State to be arranged for
  // running
  while (1) {  
    curr_onoff_tick_time = xTaskGetTickCount();
    
    // Check whether the virtual button is pressed.
    //
    // If the virtual button has been pressed for 2 or more seconds,
    // set the global flag to 1 and set the prev_tick to be equal
    // to curr_tick (clear the timer). Otherwise clear the global flag.
    //
    // If the virtual button is not pressed, clear the global flag and
    // set the prev_tick to curr_tick.
    if ((Touch_ReadX() < 1500) && (Touch_ReadY() > 600) && (Touch_ReadY() < 3000) ) { // Checks if on/off button pressed
      if ((curr_onoff_tick_time - prev_onoff_tick_time) >= 2000) {
        // Set the global flag
        onoff_status = 1;
        // Clear the timer
        prev_onoff_tick_time = curr_onoff_tick_time;
      }
      else {
        // Clear the global flag
        onoff_status = 0;
      }
    } else {
      // Clear the global flag
      onoff_status = 0;
      // Clear the timer
      prev_onoff_tick_time = curr_onoff_tick_time;
    }

    vTaskDelay(1);
  }
}

void Increase(void *p) {
  static int curr_ped_tick_time = 0;
  static int prev_ped_tick_time = 0;

  // STEP 4
  //
  // Complete the implementation of this task function.
  // It checks whether the virtual pedestrian button has been pressed
  // for 2 seconds and update the global flag accordingly.
  //
  // (Refer to instructions in the StartStop function to implement
  // this function.)
  while (1) {
    curr_ped_tick_time = xTaskGetTickCount();
    
    //Logic to check button press for 2 seconds and set flags
    if ((Touch_ReadX() > 1600) && (Touch_ReadY() > 600) && (Touch_ReadY() < 3000)) {
      if ((curr_ped_tick_time - prev_ped_tick_time) >= 2000) {
        // Set the global flag
        pedestrian_status = 1;
        // Clear the timer
        prev_ped_tick_time = curr_ped_tick_time;
      }
      else {
        // Clear the global flag
        pedestrian_status = 0;
      }
    } else {
      // Clear the global flag
      pedestrian_status = 0;
      // Clear the timer
      prev_ped_tick_time = curr_ped_tick_time;
    }
    vTaskDelay(1);
  }
}

void Decrease(void *p) {
  static int curr_light_tick_time = 0;
  static int prev_light_tick_time = 0;

  // STEP 5
  //
  // Complete the implementation of this task function.
  // It represents your Finite State Machine.
  while (1) {
    curr_light_tick_time = xTaskGetTickCount();

    // If the one of the virtual lights been lighting up for 5 or more
    // seconds, or if any of the virtual button flags has been set, switch
    // to the corresponding next state and reset the light tick.
    if ((onoff_status == 1) || (pedestrian_status == 1) || (curr_light_tick_time - prev_light_tick_time >= 5000) ) {  // replace this condition checking with something else
      // Reset the light tick
      prev_light_tick_time = curr_light_tick_time;
      // Switch to next state
      FSM();
    }

    vTaskDelay(1);
  }
}

//Function FSM sets up our Traffic System FSM to manipulate the LCD traffic lights with the virtual button
//The function is void so does not return any value.
void FSM(void) {
  // STEP 6
  //
  // Add your FSM implementation here.
  // This function will be called by the control task function.
  switch(TRAFF_State) {  //Transitions
   
   case idle:                                  //Initial Idle State
      
      if (onoff_status == 1) {                //If the onoff flag is one, transition to the stop state.
         TRAFF_State = stop;
      } 
      break;
          
    case go:

      if (onoff_status == 1) {                  //If the onoff flag is one, transition to the idle state to turn off lights
        TRAFF_State = idle;
      } else if (pedestrian_status == 1) {      //If the pedestrian flag is one, transition to the pedestrian state
        TRAFF_State = warn;
      } else {
        TRAFF_State = stop;                     //Else transition to stop. 
      }
      
      break;
    
    case stop:                                  //Stop state with red light in traffic system.
      
    if (onoff_status == 1) {                    //If onoff flag is 1, transition to idle state to turn off lights.
      TRAFF_State = idle;
    } else {
      TRAFF_State = go;                         //Else transition to green light.
    }  
    break;
    
    
    case warn:                                  //Warn state with yellow light in traffic system
      
      if (onoff_status == 1) {                  //If onoff flag is 1, transition to idle state to turn off lights.
        TRAFF_State = idle;
      } else {
        TRAFF_State = stop;                     //Else transition to stops state.
      }
      break;
      
      
  } //Transitions
  
  switch(TRAFF_State) { //State Actions
    case idle: 
      LCD_DrawFilledCircle(50,60,44, Color4[0]);
      LCD_DrawFilledCircle(160,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[0]);        //Turn off lights in idle state
      break;
    
    case go:
     
      LCD_DrawFilledCircle(160,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[0]);       //Set green light on in go state
      LCD_DrawFilledCircle(50,60,44, Color4[10]);
      break;
      
    case warn:
      LCD_DrawFilledCircle(50,60,44, Color4[0]);
      LCD_DrawFilledCircle(270,60,44, Color4[0]);    //Set yellow light on in warn state   
      LCD_DrawFilledCircle(160,60,44, Color4[14]);
      break;
      
    case stop: 
      LCD_DrawFilledCircle(50,60,44, Color4[0]);    
      LCD_DrawFilledCircle(160,60,44, Color4[0]);         //Set red light on in stop state
      LCD_DrawFilledCircle(270,60,44, Color4[12]);
      break;
    
  }
    
}

//Initialize the PLL
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

void PWM_init(void) {
  RCGCPWM |= 0x1;  //PWM1 PF1 Module Enable
  SYSCTL_RCGCPIO_R |= 0x20;   //System clock enabled to Port F 
}

void LED_init(void) {
}