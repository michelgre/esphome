#include "esphome/core/log.h"

#include "ProfaluxControler.h"
#include "DelayedAction.h"

namespace esphome {
namespace profalux_controler {

static const char* const TAG = "profalux-controler";

static OutputPin *PicoLed;  


void ProfaluxControler::setup() {
  PicoLed = new OutputPin(32);
}

void ProfaluxControler::loop() {
  
  // Traite les actions à faire si elles ont échu
  if (todo_list.size()>0) {
    DelayedAction *todo = todo_list.top();
    //ESP_LOGD(TAG,"loop: todo %d, clock=%ld, time=%ld",todo_list.size(), millis(), todo->get_when());
    if (todo->get_when()<=millis()) {
      todo_list.pop();
      todo->do_it();
      delete todo;
    }
  }
}

void ProfaluxControler::register_blind(ProfaluxBlind *blind) {
  ESP_LOGI(TAG, "Register blind %x", blind);
  this->blinds_.push_back(blind);
  blind->setControler(this);
  
}

int ProfaluxControler::getLedPinNumber() {
  return 32; // TODO: voir si cste
}

void ProfaluxControler::getPinNumbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber) {
  int n = 3 * (blindNumber - 1);
  if (n+2 < (sizeof(pins) / sizeof(int))) {
    upNumber = pins[n];
    stopNumber = pins[n+1];
    downNumber = pins[n+2];
  }
  else {
    // TODO
  }
}


}  // namespace profalux_controler
}  // namespace esphome
