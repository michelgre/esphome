#pragma once
#include <string>
#include <queue>

#include "esphome/core/component.h"
#include "OutputPin.h"
#include "DelayedAction.h"

#include "ProfaluxBlind.h"

using namespace esphome::mr::outputpin;

namespace esphome {
namespace profalux_controler {

class ProfaluxBlind;
class ProfaluxControler;
class BlinkData;
  
/*
class PinDelayedAction : public DelayedAction<ProfaluxControler> {
public:
  PinDelayedAction (ProfaluxControler *who, uint32_t when, DelayedAction<ProfaluxBlind>::Callback_t cb, OutputPin *pin) 
    : DelayedAction<ProfaluxBlind>(who, when, cb) {
    this->pin = pin;
  }
  OutputPin *pin;
};
*/

/*
    Gère les pins (TODO).
    Fournit les pins au remote (blind) selon son n°.
    Gère une queue des actions retardées, pour ne pas bloquer avec delay().
*/


class ProfaluxControler : public Component {
public:
  typedef std::priority_queue<GenericDelayedAction *, std::vector<GenericDelayedAction *>, GenericDelayedAction::CompareDesc> ToDoList;
  void register_blind(ProfaluxBlind *blind);
  OutputPin *get_led_pin();
  void get_pin_numbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber);
  OutputPin *get_pin(int pinNumber);
  void blink(OutputPin *pin, int ms);
  void blink(OutputPin *pin, int ms1, int ms2, int count);
 
  void setup() override;
  void loop() override;
 
  void add_todo(GenericDelayedAction *todo);
  void trace_todo();

  virtual float get_setup_priority() const { return 0.1; }
  void set_controler_id(std::string s) { controlerId = s; }
  std::string get_controler_id() { return controlerId; }
 
protected:
  void turn_off(DelayedAction<ProfaluxControler,OutputPin *> *action);
  void blinkAgain(DelayedAction<ProfaluxControler, BlinkData *> *action);
  std::string controlerId;   
  std::vector<ProfaluxBlind *> blinds_;
  int pins[18] = { 
    10, 11, 12, 
    7, 8, 9, 
    4, 5, 6, 
    27, 26, 22, 
    21, 20, 19,
    18, 17, 16    
  };
  ToDoList todo_list;
};

}  // namespace profalux_controler
}  // namespace esphome
