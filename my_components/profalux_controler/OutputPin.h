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
};
    
} // 
} //
} //