#include "range_finder.h"

#define MEASUREMENT_ERROR 65535

typedef  enum  {RF_IDLE, RF_TRIGGER, RF_ECHO}  state_t;
static  state_t  g_state;

bool  rf_start_msg;
bool  rf_stop_msg;
bool  rf_done_msg;
bool  rf_can_sleep = true;

uint32_t  rf_range_mm;

static  bool  initd = false;

uint32_t  dist;

static  Timer  ti;
static  bool  ti_inicio = false;
static  bool  ti_final = false;
static  bool  timerAt = false;

static  Timeout  anch;
static  bool  timeoutAnch = false;

static  DigitalOut  *  g_trigger;
static  InterruptIn  *  g_echo;

static void funct_ti_inicio(){
  ti_inicio = true;
}

static void funct_ti_final(){
  ti_final = true;
}

static void funct_anch(){
  timeoutAnch = true;
}

void rf_fsm(void){
  if(initd){
    if(ti_inicio){
      ti_inicio = false;
      ti.reset();
      ti.start();
    }
    if(ti_final){
      ti_final = false;
      ti.stop();
      timerAt = true;
    }
    switch (g_state) {
      case RF_TRIGGER:
        rf_start_msg = false;
        if(timeoutAnch){
          timeoutAnch = false;
          *g_trigger = 0;
          g_state = RF_ECHO;
        }
      break;
      case RF_ECHO:
        if(timerAt){
          timerAt = false;
          dist = ti.read_us() * 10 / 58;
          if(dist > 4000 || dist < 20){
            rf_range_mm = MEASUREMENT_ERROR;
            rf_done_msg = true;
          } else{
            rf_range_mm = dist;
            rf_done_msg = true;
          }
        }
        if(rf_stop_msg){
          rf_stop_msg = false;
          g_state = RF_IDLE;
        }
      break;
      default: //RF_IDLE
        rf_stop_msg = false;
        if(rf_start_msg){
          rf_start_msg = false;
          *g_trigger = 1;
          anch.detach();
          anch.attach_us(funct_anch, 1000);
          g_state = RF_TRIGGER;
        }
      break;
    }
    
    __disable_irq();
    if (!rf_start_msg && !rf_stop_msg && !timeoutAnch && !ti_inicio && !ti_final){
      rf_can_sleep = true;
    } else
      rf_can_sleep = false;
    __enable_irq();
  }
}

void rf_init(DigitalOut *trigger, InterruptIn *echo){
  if(!initd){
    initd = true;
    
    g_state = RF_IDLE;

    ti.reset();
    anch.detach();
    
    g_trigger = trigger;
    g_echo = echo;
    
    g_echo->rise(funct_ti_inicio);
    g_echo->fall(funct_ti_final);
  }
}
