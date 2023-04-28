#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);
static DigitalOut   g_ldr(LDR_PIN);
static PwmOut   g_dsl(DSL_PIN);
static PwmOut   g_dsr(DSR_PIN);

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

  cnt.attach_us(Funct_cnt, 333333);
  ledOn.attach_us(Funct_ledOn, 10000);
  
  uint16_t intensidad = g_lit.read_u16()/655;
  uint16_t tiempoLed = intensidad * 100 + 5;
  uint16_t tiempoDisp = (intensidad / 10 * 4) + 1;
  
  mux.attach_us(Funct_mux, tiempoDisp);
  
  g_dsl.pulsewidth_us(tiempoDisp);
  g_dsr.pulsewidth_us(0);
  
  g_dsl.period_us(40);
  g_dsr.period_us(40);
  
  bool aux = false;
  
  for(;;) {
    if(ticker4ms){
      ticker4ms = false;
      aux = not aux;
      if(aux){
        g_dsl.pulsewidth_us(0);
        g_disp = to_7seg(intensidad%10);
        g_dsr.pulsewidth_us(tiempoDisp);
      }
      else{
        g_dsr.pulsewidth_us(0);
        g_disp = to_7seg(intensidad/10);
        g_dsl.pulsewidth_us(tiempoDisp);
      }
    }
    
    if(ticker3ms){
      ticker3ms = false;
      intensidad = g_lit.read_u16()/655;
      tiempoLed = intensidad * 100 + 5;
      tiempoDisp = (intensidad / 10 * 4) + 1;
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
    

  } // forever
} // main()