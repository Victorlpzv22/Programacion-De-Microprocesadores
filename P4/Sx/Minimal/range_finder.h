#ifndef RANGE_FINDER_H
#define RANGE_FINDER_H

#include "mbed.h"

#define MEASUREMENT_ERROR 65535

// mensajes
  // mensajes leídos por range_finder
extern  bool      rf_start_msg;     // iniciar medida
extern  bool      rf_stop_msg;      // detener medida
  // mensajes generados por range_finder
extern  bool      rf_done_msg;      // medida completada
    // parametro del mensaje rf_done_msg
extern  uint32_t  rf_range_mm;      // medida en mm
extern  bool      rf_can_sleep;     // esta FSM puede dormir

// el autómata
void rf_fsm(void);
// para inicializar el autómata. Los parámetros de entrada son punteros
// a los objetos asociados a los pines Trigger y Echo del sensor
void rf_init(DigitalOut *trigger, InterruptIn *echo);

#endif
