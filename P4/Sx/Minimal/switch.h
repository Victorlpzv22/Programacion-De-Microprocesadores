#ifndef SWITCH_H
#define SWITCH_H

#include "mbed.h"

// mensajes
  // mensajes generados por switch
extern  bool sw_right_long_msg;    // pulsaci�n derecha larga
extern  bool sw_middle_msg;        // pulsaci�n central - Opcional
extern  bool sw_left_msg;          // pulsaci�n izquierda
extern  bool sw_can_sleep;         // esta FSM puede dormir

// el aut�mata
void    sw_fsm(void);
// para inicializar el aut�mata. El par�metro de entrada es un puntero
// al objeto InterruptIn asociado al pulsador
void    sw_init(InterruptIn *lsw, InterruptIn *msw, InterruptIn *rsw);

#endif
