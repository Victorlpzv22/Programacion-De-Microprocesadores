#include "switch.h"

typedef  enum  {SW_OFF, SW_UP_L, SW_ON_L, SW_DOWN_L, SW_UP_M, SW_ON_M, SW_DOWN_M, SW_UP_R, SW_ON_R, SW_DOWN_R} state_t;
static  state_t  g_state;

bool  sw_right_long_msg;
bool  sw_middle_msg;
bool  sw_left_msg;
bool  sw_can_sleep = true;

static  InterruptIn  *  gp_swl_pin;
static  InterruptIn  *  gp_swm_pin;
static  InterruptIn  *  gp_swr_pin;
static  Timeout  to;

static  bool  initd = false;
static  bool  volatile  swl_edge_evnt;
static  bool  volatile  swm_edge_evnt;
static  bool  volatile  swr_edge_evnt;
static  bool  volatile  to_evnt;

static void swl_edge_isr (void) {
  swl_edge_evnt = true;
  sw_can_sleep = false;
}

static void swm_edge_isr (void) {
  swm_edge_evnt = true;
  sw_can_sleep = false;
}

static void swr_edge_isr (void) {
  swr_edge_evnt = true;
  sw_can_sleep = false;
}

static void to_isr (void) {
  to_evnt = true;
  sw_can_sleep = false;
}

void sw_fsm(void){
  if(initd){
    switch (g_state){
      case SW_UP_L:
        swl_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
          g_state = SW_ON_L;
        }
      break;
      case SW_ON_L:
        swl_edge_evnt = false;
        to_evnt = false;
        if(1 == *gp_swl_pin){
          to.attach_us(to_isr, 40000);
          g_state = SW_DOWN_L;
        }
      break;
      case SW_DOWN_L:
        swl_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
          g_state = SW_OFF;
        }
      break;
      case SW_UP_M:
        swm_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
          g_state = SW_ON_M;
        }
      break;
      case SW_ON_M:
        swm_edge_evnt = false;
        to_evnt = false;
        if(1 == *gp_swm_pin){
          to.attach_us(to_isr, 40000);
          g_state = SW_DOWN_M;
        }
      break;
      case SW_DOWN_M:
        swm_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
          g_state = SW_OFF;
        }
      break;
      case SW_UP_R:
        swr_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
          to.attach_us(to_isr, 1000000);
          g_state = SW_ON_R;
        }
      break;
      case SW_ON_R:
        if(to_evnt){
          if(0 == *gp_swr_pin){
            to_evnt = false;
            sw_right_long_msg = true;
            to.attach_us(to_isr, 40000);
            g_state = SW_DOWN_R;
          } else{
            to_evnt = false;
            to.attach_us(to_isr, 40000);
            g_state = SW_DOWN_R;
          }
        }
      break;
      case SW_DOWN_R:
        swr_edge_evnt = false;
        if(to_evnt){
          to_evnt = false;
        g_state = SW_OFF;
        }
      break;
      default: //SW_OFF
        to_evnt = false;
        if(swl_edge_evnt){
          swl_edge_evnt = false;
          sw_left_msg = true;
          to.attach_us(to_isr, 40000);
          g_state = SW_UP_L;
        } else if(swm_edge_evnt){
          swm_edge_evnt = false;
          sw_middle_msg = true;
          to.attach_us(to_isr, 40000);
          g_state = SW_UP_M;
        } else if(swr_edge_evnt){
          swr_edge_evnt = false;
          to.attach_us(to_isr, 40000);
          g_state = SW_UP_R;
        } else{}
      break;
    }
    
    __disable_irq();
    if (!swl_edge_evnt && !swm_edge_evnt && !swr_edge_evnt && !to_evnt && 
      !sw_right_long_msg && !sw_middle_msg && !sw_left_msg){
      sw_can_sleep = true;
    } else
      sw_can_sleep = false;
    __enable_irq();
  }
}

void sw_init(InterruptIn *lsw, InterruptIn *msw, InterruptIn *rsw){
  if(!initd){
    initd = true;
    
    g_state = SW_OFF;
    swl_edge_evnt = false;
    swm_edge_evnt = false;
    swr_edge_evnt = false;
    
    to.detach();
    to_evnt = false;
    
    sw_right_long_msg = false;
    sw_middle_msg = false;
    sw_left_msg = false;
    
    gp_swl_pin = lsw;
    gp_swm_pin = msw;
    gp_swr_pin = rsw;
    
    gp_swl_pin->fall(swl_edge_isr);
    gp_swm_pin->fall(swm_edge_isr);
    gp_swr_pin->fall(swr_edge_isr);
  }
}
