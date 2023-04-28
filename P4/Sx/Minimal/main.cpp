#include "hardware.h"
#include "control.h"

int main(void) {
  
  hw_init();
  sw_init(&swl, &swm, &swr);
  rf_init(&trg, &ech);
  display_init(&dsl, &dsr, &anodes);
  ctrl_init(&ldl, &ldm, &ldr, &lit, &terminalPC);

  while(true) {
    
    sw_fsm();
    rf_fsm();
    display_fsm();
    ctrl_fsm();  
      
    __disable_irq();
    
    if(sw_can_sleep && rf_can_sleep && display_can_sleep && ctrl_can_sleep
        && !sw_middle_msg && !sw_left_msg && !sw_right_long_msg
        && !rf_start_msg && !rf_stop_msg && !display_on_msg 
        && !display_off_msg && !display_update_msg && !display_brightness_msg){
    
          __WFI();
        }
    __enable_irq();

    
  } // while(true)
}
