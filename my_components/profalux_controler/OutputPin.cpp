#include "OutputPin.h"

namespace esphome {
namespace mr {
namespace outputpin {

  /* Voir code généré sur output: */
OutputPin::OutputPin(int pinNumber) {
  this->set_component_source("gpio.output");
  App.register_component(this);
  this->myPin = new rp2040::RP2040GPIOPin();
  this->myPin->set_pin(pinNumber);
  this->myPin->set_inverted(false);
  this->myPin->set_flags(gpio::Flags::FLAG_OUTPUT);
  this->set_pin(this->myPin);
}
    
} // 
} //
} //