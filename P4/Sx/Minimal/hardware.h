#ifndef HARDWARE_H
#define HARDWARE_H

#include "mbed.h"
#include "pinout.h"

// pulsador
extern  InterruptIn swl;
extern  InterruptIn swm;
extern  InterruptIn swr;

// LEDs
extern  DigitalOut  ldl;
extern  DigitalOut  ldm;
extern  DigitalOut  ldr;

// sensor hc-sr04
extern  DigitalOut  trg;
extern  InterruptIn ech;

// �nodos del display de 7 segmentos
// en un int8_t son 0b-GFEDCBA
extern  BusOut      anodes;

// c�todos del display de 7 segmentos
extern  PwmOut      dsl;
extern  PwmOut      dsr;

// LDR
extern  AnalogIn    lit;

// Serial port
extern Serial terminalPC;

// inicializaci�n del hardware
void    hw_init(void);

#endif
