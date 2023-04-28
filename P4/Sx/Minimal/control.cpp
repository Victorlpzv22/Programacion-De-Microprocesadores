#include "control.h"
#include "mbed.h"
#include "switch.h"
#include "range_finder.h"
#include "display.h"
#include "hardware.h"
#include "to_7seg.h"

typedef  enum  {CTRL_OFF, CTRL_WAIT, CTRL_CALIB, CTRL_BRIGHT, CTRL_DIST}  state_t;
static  state_t  g_state;

bool  ctrl_can_sleep = true;

static  bool  brightness_calibrated;
static  bool  new_brightness_measurement;
static  uint16_t  brightness_calibration_values;
static  uint32_t  new_brightness_value;
static  uint8_t  brightness_cnt;
static  uint8_t  brightness_object;
static  uint32_t  dist_1;
static  uint32_t  dist_2;
static  uint8_t  far_object;

static  bool  initd = false;
static  DigitalOut  *  led_left;
static  DigitalOut  *  led_middle;
static  DigitalOut  *  led_right;
static  AnalogIn  *  ldr_lit;
static  Serial  *  serial_port;

static  Ticker  ti_obj;

static  Timeout  to_off;
static  Timeout  to_1;
static  Timeout  to_2;
static  Timeout  to_3;

static  bool  ti_obj_evnt;

static  bool  to_off_evnt;
static  bool  to_1_evnt;
static  bool  to_2_evnt;
static  bool  to_3_evnt;
static  bool  to_4_evnt;

static void ti_obj_isr (void) {
  ti_obj_evnt = true;
}

static void to_off_isr (void) {
  to_off_evnt = true;
}

static void to_1_isr (void) {
  to_1_evnt = true;
}

static void to_2_isr (void) {
  to_2_evnt = true;
}

static void to_3_isr (void) {
  to_3_evnt = true;
}

static void to_4_isr (void) {
  to_4_evnt = true;
}

void ctrl_fsm(void){
  switch (g_state){
    if(initd){
      case CTRL_WAIT:
        rf_done_msg = false;
        *led_middle = 0;
        *led_right = 0;
        if(sw_right_long_msg){
          sw_right_long_msg = false;
          display_off_msg = true;
          brightness_calibration_values = 0;
          brightness_calibrated = false;
          brightness_cnt = 0;
          rf_stop_msg = true;
          g_state = CTRL_OFF;
        }
        if(to_off_evnt){
          to_off_evnt = false;
          display_off_msg = true;
          brightness_calibration_values = 0;
          brightness_calibrated = false;
          brightness_cnt = 0;
          rf_stop_msg = true;
          g_state = CTRL_OFF;
        }
        if(ti_obj_evnt){
          ti_obj_evnt = false;
          if(far_object > 5 || brightness_object > 5){
            if(0 == *led_left)
              *led_left = 1;
            else
              *led_left = 0;
          }
        }
        if(sw_middle_msg){
          sw_middle_msg = false;
          *led_middle = 1;
          *led_right = 1;
          brightness_calibration_values = 0;
          brightness_calibrated = false;
          brightness_cnt = 0;
          to_1.detach();
          to_1.attach_us(to_1_isr, 500000);
          display_segs = 0x4077; //-A
          display_update_msg = true;
          g_state = CTRL_CALIB;
        }
        if(sw_left_msg){
          sw_left_msg = false;
          *led_middle = 1;
          *led_right = 1;
          if(!brightness_calibrated){
            to_1.detach();
            to_1.attach_us(to_1_isr, 500000);
            display_segs = 0x4077; //-A
            display_update_msg = true;
            g_state = CTRL_CALIB;
          } else{
            to_3.detach();
            to_3.attach_us(to_4_isr, 2000000);
            new_brightness_value = ldr_lit -> read_u16()/655;
            g_state = CTRL_BRIGHT;
          }
        }
      break;
      case CTRL_CALIB:
        sw_left_msg = false;
        sw_middle_msg=false;
        sw_right_long_msg = false;
        rf_done_msg = false;
        if(to_1_evnt && !brightness_calibrated){
          to_1_evnt = false;
          to_1.detach();
          to_1.attach_us(to_1_isr, 500000);
          brightness_calibration_values = brightness_calibration_values + ldr_lit->read_u16() / 655;
          brightness_cnt = brightness_cnt + 1;
          if(3 == brightness_cnt) 
            brightness_calibrated = true;
        }
        if(to_1_evnt && brightness_calibrated){
          to_1_evnt = false;
          to_2.detach();
          to_2.attach_us(to_2_isr, 2000000);
          display_brightness_msg = true;
          display_brightness = brightness_calibration_values / 3;
          display_segs = (to_7seg(display_brightness / 10) << 8) + to_7seg(display_brightness % 10);
          display_update_msg = true;
        }
        if(to_2_evnt){
          to_2_evnt = false;
          to_off.attach_us(to_off_isr, 30000000);
          ti_obj.attach_us(ti_obj_isr, 1500000);
          display_segs = 0x3F54; //On
          display_update_msg = true;
          g_state = CTRL_WAIT;
        }
      break;
      case CTRL_BRIGHT:
        sw_left_msg = false;
        sw_middle_msg=false;
        sw_right_long_msg = false;
        rf_done_msg = false;
        if(to_4_evnt){
          to_4_evnt = false;
          *led_middle = 0;
          *led_right = 0;
          new_brightness_measurement = true;
        }
        if(new_brightness_measurement){
          new_brightness_measurement = false;
          to_2.detach();
          if(new_brightness_value < display_brightness){
            to_2.attach_us(to_3_isr, 2000000);
            display_update_msg = true;
            display_segs = (to_7seg(11) << 8) + to_7seg(11);
          } else{
            to_2.attach_us(to_2_isr, 2000000);
            display_update_msg = true;
            if(brightness_object == 99)
              brightness_object = 0;
            else
              brightness_object = brightness_object + 1;
            display_segs = (to_7seg(brightness_object / 10) << 8) + to_7seg(brightness_object % 10);
          }
        }
        if(to_2_evnt){
          to_2_evnt = false;
          to_off.attach_us(to_off_isr, 30000000);
          ti_obj.attach_us(ti_obj_isr, 1500000);
          display_segs = 0x3F54; //On
          display_update_msg = true;
          g_state = CTRL_WAIT;
        }
        if(to_3_evnt){
          to_3_evnt=false;
          *led_middle = 0;
          *led_right = 0;
          to_2.detach();
          to_2.attach_us(to_3_isr, 2000000);
          rf_start_msg = true;
          g_state = CTRL_DIST;
        }
      break;
      case CTRL_DIST:
        sw_left_msg = false;
        sw_middle_msg = false;
        sw_right_long_msg = false;
        if(to_3_evnt && rf_done_msg){
          to_3_evnt = false;
          rf_done_msg = false;
          rf_stop_msg = true;
          dist_1 = rf_range_mm / 10;
          to_2.detach();
          if(65535 == dist_1){
            display_segs = (to_7seg(14) << 8) + to_7seg(12);
            to_2.attach_us(to_2_isr, 2000000);
          } else if(dist_1 > 99){
            display_segs = (to_7seg(15) << 8) + to_7seg(12);
            to_2.attach_us(to_2_isr, 2000000);
          } else{
            display_segs = (to_7seg(dist_1 / 10) << 8) + to_7seg(dist_1 % 10);
            to_2.attach_us(to_4_isr, 2000000);
            rf_start_msg = true;
          }
          display_update_msg = true;
        }
        if(to_4_evnt && rf_done_msg){
          to_4_evnt = false;
          rf_done_msg = false;
          rf_stop_msg = true;
          dist_2 = rf_range_mm / 10;
          if(65535 == dist_2){
            display_segs = (to_7seg(14) << 8) + to_7seg(12);
          } else if(dist_2 > 99){
            display_segs = (to_7seg(15) << 8) + to_7seg(12);
          } else{
            if(dist_2 > dist_1){
              if(far_object == 99)
                far_object = 0;
              else
                far_object = far_object + 1;
              display_segs = (to_7seg(far_object / 10) << 8) + to_7seg(far_object % 10);
            } else{
              display_segs = (to_7seg(11) << 8) + to_7seg(11);
            }
          }
          to_2.detach();
          to_2.attach_us(to_2_isr, 2000000);
          display_update_msg = true;
        }
        if(to_2_evnt){
          to_2_evnt = false;
          to_off.attach_us(to_off_isr, 30000000);
          ti_obj.attach_us(ti_obj_isr, 1500000);
          display_segs = 0x3F54; //On
          display_update_msg = true;
          g_state = CTRL_WAIT;
        }
      break;
      default: //CTRL_OFF
        sw_left_msg = false;
        sw_middle_msg = false;
        rf_done_msg = false;
        brightness_object = 0;
        far_object = 0;
        *led_left = 0;
        if(sw_right_long_msg){
          sw_right_long_msg = false;
          to_off.attach_us(to_off_isr, 30000000);
          ti_obj.attach_us(ti_obj_isr, 1500000);
          display_segs = 0x3F54;
          display_update_msg = true;
          display_brightness = 100;
          display_brightness_msg = true;
          display_on_msg = true;
          g_state = CTRL_WAIT;
        }
      break;
    }
    
    __disable_irq();
    if(!to_1_evnt && !to_2_evnt && !to_3_evnt && !to_4_evnt && !sw_right_long_msg && !sw_middle_msg && !sw_left_msg && !rf_done_msg){
      ctrl_can_sleep = true;
    } else
      ctrl_can_sleep = false;
    __enable_irq();
  }
}

void ctrl_init(DigitalOut *ldl, DigitalOut *ldm, DigitalOut *ldr, AnalogIn *lit, Serial *serialPort){
  if(!initd){
    initd = true;
    
    g_state = CTRL_OFF;
    
    brightness_calibrated = false;
    new_brightness_measurement = false;
    brightness_calibration_values = 0;
    new_brightness_value = 0;
    brightness_cnt = 0;
    brightness_object = 0;
    dist_1 = 0;
    dist_2 = 0;
    far_object = 0;
    
    to_off_evnt = false;
    to_1_evnt = false;
    to_2_evnt = false;
    to_3_evnt = false;
    to_4_evnt = false;
    
    led_left = ldl;
    led_middle = ldm;
    led_right = ldr;
    ldr_lit = lit;
    serial_port = serialPort;
    
    *led_left = 0;
    *led_middle = 0;
    *led_right = 0;
  }
}
