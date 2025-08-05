#include "GPIO_DEAKIN.h"
#include <stddef.h>  // for NULL definition (used for invalid ports)


// Returns the correct PortGroup pointer for a given port letter ('A' or 'B')
static PortGroup* GetPort(char portChar) {
    switch (portChar) {
        case 'A': // Port A
        case 'a':
            return &PORT->Group[0];
        case 'B': // Port B
        case 'b':
            return &PORT->Group[1];
        default:  // Invalid port
            return NULL;
    }
}


// Configures a pin as INPUT or OUTPUT on the given port
bool Config_GPIO(char PortNum, char PinNum, char Mode) {
    PortGroup* port = GetPort(PortNum);   // Get port base address
    if (port == NULL || PinNum > 31)      // Check valid port and pin
        return false;

    // Enable PORT clock (ensures GPIO registers are powered)
    PM->APBBMASK.reg |= PM_APBBMASK_PORT;

    // Configure pin as GPIO (disable peripheral multiplexer)
    // Enable input buffer so pin can read/write
    port->PINCFG[PinNum].reg = PORT_PINCFG_INEN;

    // Set pin direction
    if (Mode == OUTPUT) {
        port->DIRSET.reg = (1 << PinNum); // Set bit = output
    } else {
        port->DIRCLR.reg = (1 << PinNum); // Clear bit = input
    }
    return true;
}


// Sets a pin HIGH or LOW
bool Write_GPIO(char PortNum, char PinNum, bool State) {
    PortGroup* port = GetPort(PortNum);   // Get port base address
    if (port == NULL || PinNum > 31)      // Check valid port and pin
        return false;

    if (State == HIGH) {
        port->OUTSET.reg = (1 << PinNum); // Set bit = logic HIGH
    } else {
        port->OUTCLR.reg = (1 << PinNum); // Clear bit = logic LOW
    }
    return true;
}


// Reads the current state of a pin (returns HIGH or LOW)
bool Read_GPIO(char PortNum, char PinNum) {
    PortGroup* port = GetPort(PortNum);   // Get port base address
    if (port == NULL || PinNum > 31)      // Check valid port and pin
        return LOW; // Default to LOW if invalid

    // Check bit in IN register (input value)
    return (port->IN.reg & (1 << PinNum)) ? HIGH : LOW;
}
