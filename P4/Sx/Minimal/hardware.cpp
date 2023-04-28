#include "hardware.h"

#define BAUD_RATE 57600

// switch
InterruptIn swl(SWL_PIN);
InterruptIn swm(SWM_PIN);
InterruptIn swr(SWR_PIN);

// LEDs
DigitalOut  ldl(LDL_PIN);
DigitalOut  ldm(LDM_PIN);
DigitalOut  ldr(LDR_PIN);

// hc-sr04 range finder stuff
DigitalOut  trg(TRG_PIN);
InterruptIn ech(ECH_PIN);

// seven segment display anodes
// when in a int8_t, they are 0b-GFEDCBA
BusOut      anodes(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN,
                    SGE_PIN, SGF_PIN, SGG_PIN);

// display cathodes
PwmOut      dsl(DSL_PIN);
PwmOut      dsr(DSR_PIN);

// LDR
AnalogIn    lit(LIT_PIN);

// Serial port
Serial terminalPC(USBTX, USBRX);

static volatile char serialRx;

void terminalPC_isr(){serialRx = terminalPC.getc();};

// hardware initialization
void hw_init(void) {
  swl.mode(PullUp);
  swm.mode(PullUp);
  swr.mode(PullUp);
  ech.mode(PullDown);
  
  terminalPC.baud(BAUD_RATE);
  terminalPC.format(8, terminalPC.None, 1);
  terminalPC.attach(terminalPC_isr, terminalPC.RxIrq);
}
