/**
 * EE/CSE 474: Lab3 header file
 */

#ifndef _PWM_HEADER_H_
#define _PWM_HEADER_H_

#include <stdint.h>

// ******************** CLOCK CONTROL REGISTERS ********************** //
#define RCGCPWM                 (*(volatile uint32_t *) 0x400FE640)
#define PWMCC                   (*(volatile uint32_t *) 0x40028FC8)
#define PWMGENA                 (*(volatile uint32_t *) 0x400280A0)
#define PWMGENB                 (*(volatile uint32_t *) 0x400280A4)
#define PWMLOAD                 (*(volatile uint32_t *) 0x40028090)
#define PWMCMPA                 (*(volatile uint32_t *) 0x40028098)
#define PWMCMPB                 (*(volatile uint32_t *) 0x4002809C)
#define PWMCTL                  (*(volatile uint32_t *) 0x40028080)
#define PWMENABLE               (*(volatile uint32_t *) 0x40028008)

#endif  // _PWM_HEADER_H_