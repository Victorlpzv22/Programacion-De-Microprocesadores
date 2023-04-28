#ifndef DISPLAY_H
#define DISPLAY_H

#include "mbed.h"

// mensajes
  // mensajes le�dos por display
extern bool      display_on_msg;          // encender el display
extern bool      display_off_msg;         // apagar el display
extern bool      display_update_msg;      // modificar el texto del display
    // par�metro del mensaje display_update_msg
    // los 8 LSB indican los segmentos en el display derecho, los 8 MSB
    // los del izquierdo. El orden de los segmentos es 0b-GFEDCBA
    // el texto por defecto son dos blancos
extern uint16_t  display_segs;
extern bool      display_brightness_msg;  // modificar el brillo
    // par�metro del mensaje display_brightness_msg
extern uint8_t   display_brightness;      // en % de 0 a 100. Por defecto es 100 %
  // mensaje generado por el display
extern bool      display_can_sleep;       // esta FSM puede dormir

// el aut�mata
void display_fsm(void);
// para inicializar la FSM. left y right son los c�todos, anodes son los
// segmentos en orden 0b-GFEDCBA
void display_init(PwmOut *left, PwmOut *right, BusOut *anodes);

#endif
