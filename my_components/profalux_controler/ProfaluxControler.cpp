#include "esphome/core/log.h"

#include "ProfaluxControler.h"
#include "DelayedAction.h"
#include "OutputPin.h"

using namespace esphome::mr::outputpin;

namespace esphome {
namespace profalux_controler {

static const char* const TAG = "profalux-controler";

#define PIN_COUNT 29

  static int CanPin[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 0, 0, 0, 1, 1, 1
};

static OutputPin *AllPins[PIN_COUNT];
static OutputPin *PicoLed;  

void ProfaluxControler::setup() {
  PicoLed = new OutputPin(32);  // TODO: get # from platform
  for (int i=0; i<PIN_COUNT;i++) {
    if (CanPin[i]) {
      AllPins[i] = new OutputPin(i);
    }
    else {
      AllPins[i] = NULL;
    }
  }
}

OutputPin *ProfaluxControler::get_pin(int pinNumber) {
  if (pinNumber < PIN_COUNT) {
    return AllPins[pinNumber];
  }
  else {
    return NULL;
  }
}


void ProfaluxControler::add_todo(GenericDelayedAction *todo) { 
  todo_list.push(todo);
  trace_todo();
}

void ProfaluxControler::trace_todo() {
  ToDoList pq (todo_list);
  while (pq.size()>0) {
    GenericDelayedAction *todo = pq.top();
    pq.pop();
    ESP_LOGD(TAG, "Todo List: %ld", todo->get_when());
  }
}

void ProfaluxControler::loop() {
  
  // Traite les actions à faire si elles ont échu
  if (todo_list.size()>0) {
    while (todo_list.size()>0) {
      GenericDelayedAction *todo = todo_list.top();
      //ESP_LOGD(TAG,"loop: todo %d, clock=%ld, time=%ld",todo_list.size(), millis(), todo->get_when());
      if (todo->get_when()<=millis()) {
        todo_list.pop();
        todo->do_it();
        delete todo;
      }
      else {
        break; // Plus rien à faire pour l'instant
      }
    }
  }
}

void ProfaluxControler::register_blind(ProfaluxBlind *blind) {
  ESP_LOGI(TAG, "Register blind %x", blind);
  this->blinds_.push_back(blind);
  blind->setControler(this);
  
}

OutputPin *ProfaluxControler::get_led_pin() {
  return PicoLed;
}

void ProfaluxControler::get_pin_numbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber) {
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

void ProfaluxControler::turn_off(DelayedAction<ProfaluxControler, OutputPin *> *action) {
  action->get_data()->turn_off();
}

void ProfaluxControler::blink(OutputPin *pin, int ms) {
  if (pin!=NULL) {
    pin->turn_on();
    DelayedAction<ProfaluxControler, OutputPin *>::Callback_t pMethod = &ProfaluxControler::turn_off;
    DelayedAction<ProfaluxControler, OutputPin *> *off = new DelayedAction(this, millis()+ms, pMethod, pin);
    this->add_todo(off);
  }
}  

struct BlinkData {
  BlinkData(OutputPin *pin, int ms1, int ms2, int count, bool isOn) {
    this->pin = pin;
    this->ms1 = ms1;
    this->ms2 = ms2;
    this->count = count;
    this->isOn = isOn;
  }
  OutputPin *pin;
  int ms1;
  int ms2;
  int count;
  bool isOn;
};

void ProfaluxControler::blinkAgain(DelayedAction<ProfaluxControler, BlinkData *> *action) {
  BlinkData *data = action->get_data() ;
  if (!data->isOn) {
    data->pin->turn_on();
    data->isOn = true;
    this->add_todo(new DelayedAction<ProfaluxControler, BlinkData *>(this, millis()+data->ms1, &ProfaluxControler::blinkAgain, data));
  }
  else {
    data->pin->turn_off();
    data->isOn = false;
    data->count--;
    if (data->count==0) {
      delete data;
    }
    else {
      this->add_todo(new DelayedAction<ProfaluxControler, BlinkData *>(this, millis()+data->ms2, &ProfaluxControler::blinkAgain, data));
    }
  }
}
  
void ProfaluxControler::blink(OutputPin *pin, int ms1, int ms2, int count) {
  if (pin != NULL) {
    pin->turn_on();
    BlinkData *data = new BlinkData(pin, ms1, ms2, count, true);
    DelayedAction<ProfaluxControler, BlinkData *> *blink = new DelayedAction(this, millis()+ms1, &ProfaluxControler::blinkAgain, data);
    this->add_todo(blink);
  }
}
}  // namespace profalux_controler
}  // namespace esphome
