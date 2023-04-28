#ifndef CONTROL_H
#define CONTROL_H

#include "switch.h"
#include "range_finder.h"
#include "display.h"


extern bool ctrl_can_sleep;         // esta FSM puede dormir

// mensajes le�dos por control
// todos ellos est�n declarados en los .h anteriores
// generado por control

// el aut�mata
void ctrl_fsm(void);
// para inicializar la FSM. Los par�metros son punteros a las se�ales
// leidas o controladas directamente por control
void ctrl_init(DigitalOut *ldl, DigitalOut *ldm, DigitalOut *ldr, AnalogIn *lit, Serial *serialPort);

#endif
