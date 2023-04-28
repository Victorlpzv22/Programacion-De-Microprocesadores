#include "pinout.h"
#include "sw_tick_serial.h"
#include "to_7seg.h"

#if 0
# define VERBOSE
#endif

static BusOut   g_disp(SGG_PIN, SGF_PIN, SGE_PIN, SGD_PIN, SGC_PIN, SGB_PIN, SGA_PIN);
static BusOut   g_leds(LDR_PIN, LDM_PIN, LDL_PIN);
static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

int main (void) {
  sw_tick_serial_init();
  
  int cont = 0;
  
  g_dsl = 1;
  g_dsr = 0;
  g_leds = 4;
  
  for(;;){
    if (gb_tick_1ms_evnt) {
      gb_tick_1ms_evnt = false;

      if(1 == g_dsl){
        g_dsl = 0;
        g_disp = to_7seg(cont);
        g_dsr = 1;
      }

      else{
        g_dsr = 0;
        g_disp = to_7seg(10);
        g_dsl = 1;
      }
    }
    
    if(gb_swl_evnt){
      gb_swl_evnt = false;
      g_disp = to_7seg(cont);
      cont++;
      if(cont == 10)
        cont = 9;
      #ifdef VERBOSE
      printf("Boton izquierdo n%d\n\r", cont);
      #endif // VERBOSE
    }
    
    if(gb_swm_evnt){
      gb_swm_evnt = false;
      g_disp = to_7seg(cont);
      cont--;
      if(cont == -1)
        cont = 0;
      #ifdef VERBOSE
      printf("Boton central n%d\n\r", cont);
      #endif // VERBOSE
    }
    
    if(gb_swr_evnt){
      gb_swr_evnt = false;
      cont = 0;
      g_disp = to_7seg(cont);
      #ifdef VERBOSE
      printf("Boton derecho, contador reseteado\n\r");
      #endif // VERBOSE
    }
    
    if (gb_tick_100ms_evnt) {
      gb_tick_100ms_evnt = false;
      g_leds = ((4 == g_leds) ? 1 : (g_leds << 1));
    }
  }
}
