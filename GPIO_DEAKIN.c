#include "GPIO_DEAKIN.h"
#include <stddef.h>  // for NULL definition


static PortGroup* GetPort(char portChar) {
    switch (portChar) {
        case 'A':
        case 'a':
            return &PORT->Group[0];
        case 'B':
        case 'b':
            return &PORT->Group[1];
        default:
            return NULL;
    }
}

bool Config_GPIO(char PortNum, char PinNum, char Mode) {
    PortGroup* port = GetPort(PortNum);
    if (port == NULL || PinNum > 31) return false;

    // Enable PORT clock
    PM->APBBMASK.reg |= PM_APBBMASK_PORT;

    // Disable peripheral mux to set as GPIO and enable input buffer
    port->PINCFG[PinNum].reg = PORT_PINCFG_INEN;  // input enabled, PMUX disabled

    // Set direction
    if (Mode == OUTPUT) {
        port->DIRSET.reg = (1 << PinNum);
    } else {
        port->DIRCLR.reg = (1 << PinNum);
    }
    return true;
}


bool Write_GPIO(char PortNum, char PinNum, bool State) {
    PortGroup* port = GetPort(PortNum);
    if (port == NULL || PinNum > 31) return false;

    if (State == HIGH) {
        port->OUTSET.reg = (1 << PinNum);
    } else {
        port->OUTCLR.reg = (1 << PinNum);
    }
    return true;
}

bool Read_GPIO(char PortNum, char PinNum) {
    PortGroup* port = GetPort(PortNum);
    if (port == NULL || PinNum > 31) return LOW;

    return (port->IN.reg & (1 << PinNum)) ? HIGH : LOW;
}
