#pragma once

#include "esphome/components/rp2040/core.h"
#include "esphome/components/rp2040/gpio.h"
#include "esphome/components/rp2040/preferences.h"
#include "esphome/components/gpio/output/gpio_binary_output.h"
#include "esphome/core/application.h"

namespace esphome {
namespace mr {
namespace outputpin {
  
class OutputPin : public gpio::GPIOBinaryOutput {
 protected:
  rp2040::RP2040GPIOPin *myPin = NULL;
  
 public:
  OutputPin(int pinNumber);
  int getPinNumber() { return myPin->get_pin(); }
  void blink(int ms) { turn_on(); delay(ms); turn_off(); }
  void blink(int mso, int msd) { turn_on(); delay(mso); turn_off(); delay(msd); }
  void blink(int mso, int msd, int cnt) {
    for (int i=0; i<cnt; i++) {
      turn_on();
      delay(mso);
      turn_off();
      if (i<(cnt-1)) {
        delay(msd);
      }
    }
  }
};
    
} // 
} //
} //