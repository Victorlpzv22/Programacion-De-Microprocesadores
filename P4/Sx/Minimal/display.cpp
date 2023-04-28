#include "display.h"

typedef enum {DISP_OFF, DISP_ON, DISP_MUX} state_t;
static state_t g_state;

bool  display_on_msg;
bool  display_off_msg;
bool  display_update_msg;
bool  display_brightness_msg;
bool  display_can_sleep = true;

uint16_t  display_segs;
uint8_t  display_brightness;

static  bool  initd = false;
static  BusOut  *  g_seven_seg;
static  PwmOut  *  g_dsr;
static  PwmOut  *  g_dsl;
static  Ticker  mux;

static  bool  volatile  aux;
static  bool  volatile  mux_evnt;

static void mux_isr (void) {
  mux_evnt = true;
  display_can_sleep = false;
}

void display_fsm(void) {
  if(initd){
    switch (g_state) {
      case DISP_MUX:
        display_on_msg = false;
        display_update_msg = false;
        display_brightness_msg = false;
        aux = not aux;
        if(display_off_msg){
          display_off_msg = false;
          g_dsl->pulsewidth_us(0);
          g_dsr->pulsewidth_us(0);
          g_state = DISP_OFF;
        }
        if(mux_evnt){
          mux_evnt = false;
          g_state = DISP_ON;
        }
      break;
      case DISP_ON:
        display_on_msg = false;
        if(aux){
          g_dsl->pulsewidth_us(0);
          *g_seven_seg = display_segs;
          g_dsr->pulsewidth_us((display_brightness / 10 * 4) + 1);
        } else{
          g_dsr->pulsewidth(0);
          *g_seven_seg = display_segs >> 8;
          g_dsl->pulsewidth_us((display_brightness / 10 * 4) + 1);
        }
        if(mux_evnt){
          mux_evnt = false;
          g_state = DISP_MUX;
        }
        if(display_update_msg){
          display_update_msg = false;
        }
        if(display_brightness_msg){
          display_brightness_msg = false;
        }
        if(display_off_msg){
          display_off_msg = false;
          g_dsl->pulsewidth_us(0);
          g_dsr->pulsewidth_us(0);
          g_state = DISP_OFF;
        }
      break;
      default: //DISP_OFF
        display_off_msg = false;
        display_update_msg = false;
        display_brightness_msg = false;
        if (display_on_msg) {
          display_on_msg = false;
          g_state = DISP_ON;
        }
      break;
    }
    
    __disable_irq();
    if (!display_on_msg && !display_off_msg && 
      !display_update_msg && !display_brightness_msg && !mux_evnt){
      display_can_sleep = true;
    } else
      display_can_sleep = false;
    __enable_irq();
  }
}

void display_init(PwmOut *left, PwmOut *right, BusOut *anodes){
  if(!initd){
    initd = true;
    
    aux = false;
    
    g_state = DISP_OFF;
    mux_evnt = false;
    mux.attach_us(mux_isr, 4000);
    
    g_dsr = right;
    g_dsl = left;
    g_seven_seg = anodes;
    
    g_dsr->period_us(40);
    g_dsl->period_us(40);
  }
}
