#ifndef CONTROL_H
#define CONTROL_H

#include "switch.h"
#include "range_finder.h"
#include "display.h"


extern bool ctrl_can_sleep;         // esta FSM puede dormir

// mensajes leídos por control
// todos ellos están declarados en los .h anteriores
// generado por control

// el autómata
void ctrl_fsm(void);
// para inicializar la FSM. Los parámetros son punteros a las señales
// leidas o controladas directamente por control
void ctrl_init(DigitalOut *ldl, DigitalOut *ldm, DigitalOut *ldr, AnalogIn *lit, Serial *serialPort);

#endif
