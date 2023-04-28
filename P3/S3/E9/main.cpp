#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

static BusOut   g_disp(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN, SGE_PIN, SGF_PIN, SGG_PIN);

static DigitalOut   g_dsl(DSL_PIN);
static DigitalOut   g_dsr(DSR_PIN);

static DigitalOut   trigger(TRG_PIN);
static InterruptIn   echo(ECH_PIN);

static Ticker mux;
static bool ticker4ms = false;

static void Funct_mux(){
  ticker4ms = true;
}

static Ticker med;
static bool tickerMed = false;

static void Funct_med(){
  tickerMed = true;
}

static Timeout anch;
static bool timeoutAnch = false;

static void Funct_anch(){
  timeoutAnch = true;
}

static Timer At;
static bool timerAt = false;

static bool inicio = false;

static void Funct_inicio(){
  inicio = true;
}

static bool final = false;

static void Funct_final(){
  final = true;
}

int main(void) {
  mux.attach_us(Funct_mux, 4000);
  med.attach_us(Funct_med, 100000);
  
  echo.rise(Funct_inicio);
  echo.fall(Funct_final);
  
  g_dsl = 1;
  g_dsr = 0;
  
  uint8_t dist = 0;
  
  for(;;) {
    if(ticker4ms) {
      ticker4ms = false;
      
      if(1 == g_dsl){
        g_dsl = 0;
        if(dist < 100)
          g_disp = to_7seg(dist%10);
        else
          g_disp = to_7seg(11);
        g_dsr = 1;
      }
      else{
        g_dsr = 0;
        if(dist < 100)
          g_disp = to_7seg(dist/10);
        else
          g_disp = to_7seg(11);
        g_dsl = 1;
      }
    }
    
    if(inicio){
      inicio = false;
      At.reset();
      At.start();
    }
    
    if(final){
      final = false;
      At.stop();
      timerAt = true;
    }
    
    if(tickerMed){
      tickerMed = false;
      trigger = 1;
      anch.attach_us(Funct_anch, 1000);
    }
    
    if(timeoutAnch){
      timeoutAnch = false;
      trigger = 0;
    }
    
    if(timerAt){
      timerAt = false;
      dist = At.read_us()/58;
    }
    
    __disable_irq();
    if(!ticker4ms && !tickerMed && !timeoutAnch && !inicio && !final){
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()
