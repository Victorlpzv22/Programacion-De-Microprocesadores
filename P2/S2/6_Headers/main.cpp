#include "pinout.h"
#include "sw_tick_serial.h"
#include "to_7seg.h"

static BusOut   g_disp(SGG_PIN, SGF_PIN, SGE_PIN, SGD_PIN, SGC_PIN, SGB_PIN, SGA_PIN);
static BusOut   g_leds(LDR_PIN, LDM_PIN, LDL_PIN);
static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

int main (void) {
  sw_tick_serial_init();
  
  int cont = 1;
  
  g_dsl = 0;
  g_dsr = 1;
  g_leds = 4;
  g_disp = to_7seg(0);
  
  for(;;){
    if(gb_swl_evnt){
      gb_swl_evnt = false;
      g_disp = to_7seg(cont);
      cont++;
      if(cont == 16)
        cont = 0;
    }
    if(gb_swm_evnt){
      gb_swm_evnt = false;
      int provisional = g_dsl;
      g_dsl = g_dsr;
      g_dsr = provisional;
    }
    if(gb_swr_evnt){
      gb_swr_evnt = false;
      g_leds = ((4 == g_leds) ? 1 : (g_leds << 1));
    }
  }
}
