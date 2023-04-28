#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);
static PwmOut   g_dsl(DSL_PIN);
static PwmOut   g_dsr(DSR_PIN);

static AnalogIn   g_lit(LIT_PIN);

static InterruptIn   g_swl(SWL_PIN);
static InterruptIn   g_swm(SWM_PIN);
static InterruptIn   g_swr(SWR_PIN);

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

Timeout pulsIzquierdo;
bool volatile  pulsIzdo;

void Funct_pulsIzdo(){
  pulsIzdo = true;
}

void pulsadoIzdo(){
  pulsIzquierdo.attach_us(Funct_pulsIzdo, 40000);
}

Timeout pulsCentro;
bool volatile  pulsCntr;

void Funct_pulsCntr(){
  pulsCntr = true;
}

void pulsadoCntr(){
  pulsCentro.attach_us(Funct_pulsCntr, 40000);
}

Timeout pulsDerecho;
bool volatile  pulsDcho;

void Funct_pulsDcho(){
  pulsDcho = true;
}

void pulsadoDcho(){
  pulsDerecho.attach_us(Funct_pulsDcho, 40000);
}

int main(void) {
  cnt.attach_us(Funct_cnt, 333333);
  
  g_swl.mode(PullUp);
  g_swl.fall(pulsadoIzdo);
  
  g_swm.mode(PullUp);
  g_swm.fall(pulsadoCntr);
  
  g_swr.mode(PullUp);
  g_swr.fall(pulsadoDcho);
  
  uint16_t intensidad = g_lit.read_u16()/655;
  uint16_t tiempoDisp = (intensidad / 10 * 4) + 1;
  
  mux.attach_us(Funct_mux, 4000);
  
  g_dsl.pulsewidth_us(tiempoDisp);
  g_dsr.pulsewidth_us(0);
  
  g_dsl.period_us(40);
  g_dsr.period_us(40);
  
  uint8_t cont = 0;
  
  bool aux = false;
  
  for(;;) {
    if(ticker4ms){
      ticker4ms = false;
      aux = not aux;
      if(aux){
        g_dsl.pulsewidth_us(0);
        g_disp = to_7seg(cont%10);
        g_dsr.pulsewidth_us(tiempoDisp);
      }
      else{
        g_dsr.pulsewidth_us(0);
        g_disp = to_7seg(cont/10);
        g_dsl.pulsewidth_us(tiempoDisp);
      }
    }
    
    if(ticker3ms){
      ticker3ms = false;
      intensidad = g_lit.read_u16()/655;
      tiempoDisp = (intensidad / 10 * 4) + 1;
    }
    
    if(pulsIzdo){
      pulsIzdo = false;
      if(1 == g_swl){
        if(0 == cont)
          cont = 99;
        else
          cont--;
      }
    }
    
    if(pulsCntr){
      pulsCntr = false;
      if(1 == g_swm){
        cont = 99 - cont;
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
    if(!ticker4ms && !ticker3ms && !pulsIzdo && !pulsCntr && !pulsDcho && !g_swl && !g_swm && !g_swr){
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()
