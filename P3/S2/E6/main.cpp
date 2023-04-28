#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);
static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);
static InterruptIn   g_swr(SWR_PIN);

Ticker mux;
bool ticker4ms = false;

void Funct_mux(){
  ticker4ms = true;
}

Timeout puls;
bool volatile  pulsDcho;

void Funct_pulsDcho(){
  pulsDcho = true;
}

void pulsado(){
  puls.attach_us(Funct_pulsDcho, 40000);
}

int main(void) {
  mux.attach_us(Funct_mux, 4000);
  
  g_swr.mode(PullUp);
  g_swr.fall(pulsado);
  
  g_dsl = 1;
  g_dsr = 0;
  
  uint8_t cont = 0;
  
  for(;;) {
    if(ticker4ms) {
      ticker4ms = false;
      
      if(1 == g_dsl){
        g_dsl = 0;
        g_disp = to_7seg(cont%10);
        g_dsr = 1;
      }
      else{
        g_dsr = 0;
        g_disp = to_7seg(cont/10);
        g_dsl = 1;
      }
    }
    
    if(pulsDcho){
      pulsDcho = false;
      if(1 == g_swr){
        cont++;
        if(cont > 99)
          cont = 0;
      }
    }
    
    __disable_irq();
    if(!ticker4ms && !pulsDcho && !g_swr){
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()
