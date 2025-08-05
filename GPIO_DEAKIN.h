#ifndef GPIO_DEAKIN_H
#define GPIO_DEAKIN_H

#include <stdbool.h>  // For using 'bool' type
#include "sam.h"      // CMSIS header for SAMD21 microcontroller registers


// Used in Config_GPIO() to set pin direction
#define INPUT  0   // Configure pin as input
#define OUTPUT 1   // Configure pin as output

// Used in Write_GPIO() and Read_GPIO() for pin logic levels
#define LOW    0   // Logic LOW (0V) 
#define HIGH   1   // Logic HIGH (3.3V on Nano 33 IoT)


// Configures a GPIO pin as INPUT or OUTPUT on a specified port
bool Config_GPIO(char PortNum, char PinNum, char Mode);

// Writes a logic state (HIGH or LOW) to a specified GPIO pin
bool Write_GPIO(char PortNum, char PinNum, bool State);

// Reads the logic state (HIGH or LOW) from a specified GPIO pin
bool Read_GPIO(char PortNum, char PinNum);

#endif
