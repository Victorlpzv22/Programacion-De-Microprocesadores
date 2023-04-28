#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);
static DigitalOut   g_ldr(LDR_PIN);
static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

static AnalogIn   g_lit(LIT_PIN);

Ticker mux;
bool ticker4ms = false;

void Funct_mux(){
  ticker4ms = true;
}

Ticker cnt;
bool ticker3ms = false;

void Funct_cnt(){
  ticker3ms = true;
}

Ticker ledOn;
bool tickerOn = false;

void Funct_ledOn(){
  tickerOn = true;
}

Timeout ledOff;
bool  tickerOff = false;

void Funct_ledOff(){
  tickerOff = true;
}

int main(void) {
  mux.attach_us(Funct_mux, 4000);
  cnt.attach_us(Funct_cnt, 333333);
  ledOn.attach_us(Funct_ledOn, 10000);
  
  g_dsl = 1;
  g_dsr = 0;
  
  uint16_t intensidad = g_lit.read_u16()/655;
  uint16_t tiempoLed = intensidad * 100 + 5;
  
  for(;;) {
    if(ticker4ms){
      ticker4ms = false;
      
      if(1 == g_dsl){
        g_dsl = 0;
        g_disp = to_7seg(intensidad%10);
        g_dsr = 1;
      }
      else{
        g_dsr = 0;
        g_disp = to_7seg(intensidad/10);
        g_dsl = 1;
      }
    }
    
    if(ticker3ms){
      ticker3ms = false;
      intensidad = g_lit.read_u16()/655;
      tiempoLed = intensidad * 100 + 5;
    }
    
    if(tickerOn){
      tickerOn = false;
      g_ldr = 1;
      ledOff.attach_us(Funct_ledOff, tiempoLed);
    }
    
    if(tickerOff){
      tickerOff = false;
      g_ldr = 0;
    }
    
    if(!ticker4ms && !ticker3ms){
      __WFI();
    }
  } // forever
} // main()
