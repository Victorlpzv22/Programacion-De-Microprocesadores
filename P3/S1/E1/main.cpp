#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);
static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

Ticker mux;
bool ticker4ms = false;

void Funct_mux(){
  ticker4ms = true;
}

int main(void) {
  mux.attach_us(Funct_mux, 4000);
  
  g_dsl = 1;
  g_dsr = 0;
  
  for(;;) {
    if(!ticker4ms){
      __WFI();
    }
    
    if(ticker4ms) {
      ticker4ms = false;
      
      if(1 == g_dsl){
        g_dsl = 0;
        g_disp = to_7seg(13);
        g_dsr = 1;
      }
      else{
        g_dsr = 0;
        g_disp = to_7seg(0);
        g_dsl = 1;
      }
    }
  } // forever
} // main()
