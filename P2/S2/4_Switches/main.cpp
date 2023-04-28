#include "mbed.h"
#include "pinout.h"
#include "sw_tick_serial.h"

static BusOut   g_disp(SGG_PIN, SGF_PIN, SGE_PIN, SGD_PIN, SGC_PIN, SGB_PIN, SGA_PIN);
static BusOut   g_leds(LDR_PIN, LDM_PIN, LDL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

int8_t to_7seg(uint8_t code){
  int8_t const sseg[] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 123};
  if(0 <= code && code <= 9)
    return sseg[code];
  else
    return 0;
}

int main (void) {
  sw_tick_serial_init();
  
  int cont = 1;
  
  g_dsr = 1;
  g_leds = 4;
  g_disp = to_7seg(0);
  
  for(;;){
    if(gb_swl_evnt){
      gb_swl_evnt = false;
      g_disp = to_7seg(cont);
      cont++;
      if(cont == 10)
        cont = 0;
    }
    if(gb_swr_evnt){
      gb_swr_evnt = false;
      g_leds = ((4 == g_leds) ? 1 : (g_leds << 1));
    }
  }
}
