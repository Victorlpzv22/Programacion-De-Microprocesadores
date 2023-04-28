#ifndef SWITCH_H
#define SWITCH_H

#include "mbed.h"

// mensajes
  // mensajes generados por switch
extern  bool sw_right_long_msg;    // pulsación derecha larga
extern  bool sw_middle_msg;        // pulsación central - Opcional
extern  bool sw_left_msg;          // pulsación izquierda
extern  bool sw_can_sleep;         // esta FSM puede dormir

// el autómata
void    sw_fsm(void);
// para inicializar el autómata. El parámetro de entrada es un puntero
// al objeto InterruptIn asociado al pulsador
void    sw_init(InterruptIn *lsw, InterruptIn *msw, InterruptIn *rsw);

#endif
