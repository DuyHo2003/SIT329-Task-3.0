#ifndef GPIO_DEAKIN_H
#define GPIO_DEAKIN_H

#include <stdbool.h>
#include "sam.h"  // CMSIS header for SAMD21

// Mode definitions
#define INPUT  0
#define OUTPUT 1

// State definitions
#define LOW    0
#define HIGH   1

bool Config_GPIO(char PortNum, char PinNum, char Mode);
bool Write_GPIO(char PortNum, char PinNum, bool State);
bool Read_GPIO(char PortNum, char PinNum);

#endif
