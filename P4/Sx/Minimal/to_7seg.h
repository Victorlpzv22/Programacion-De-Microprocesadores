#ifndef TO_7SEG_H
#define TO_7SEG_H
#include "mbed.h"

#define LETTER_A  (10U)
#define HYPHEN    (11U)
#define LETTER_r  (12U)
#define LETTER_n  (13U)
#define LETTER_E  (14U)
#define LETTER_F  (15U)

// convierte a 7 segmentos (activos altos, en orden GFEDCBA)
// los códigos de entrada desde 0 a 15 dan lugar a los símbolos:
//  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, -, r, n, E, F}
// otras entradas dan lugar a un display apagado
int8_t to_7seg(uint8_t x);

#endif // TO_7SEG_H
