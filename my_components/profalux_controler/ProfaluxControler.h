#pragma once
#include <string>

#include "esphome/core/component.h"
#include "ProfaluxBlind.h"



namespace esphome {
namespace profalux_controler {

class ProfaluxBlind;
class DelayedAction;
  
/*
    Gère les pins (TODO).
    Fournit les pins au remote (blind) selon son n°.
    Gère une queue des actions retardées, pour ne pas bloquer avec delay().
*/


class ProfaluxControler : public Component {
 public:
  void register_blind(ProfaluxBlind *blind);
  int getLedPinNumber();
  void getPinNumbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber);
 
  void setup() override;
  void loop() override;
 
  void add_todo(DelayedAction *todo) { todo_list.push(todo); }

  virtual float get_setup_priority() const { return 0.1; }
  void set_controler_id(std::string s) { controlerId = s; }
  std::string get_controler_id() { return controlerId; }
 
 protected:
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
  std::priority_queue<DelayedAction *> todo_list;
};

}  // namespace profalux_controler
}  // namespace esphome
