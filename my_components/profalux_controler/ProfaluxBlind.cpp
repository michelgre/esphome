#include "esphome/core/log.h"
#include "esphome.h"

#include "ProfaluxBlind.h"
#include "DelayedAction.h"

namespace esphome {
namespace profalux_controler {

using namespace api;
using namespace output;
using namespace mr::outputpin;

#define PIN_COUNT 29
static const char* const TAG = "profalux-blind";

/* TODO
   ====
  
   Mettre la creation des pins dans Controler.
   Le volet va récupérer les pins qui le concernent, selon son n°.
*/
  
static int CanPin[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 0, 0, 0, 1, 1, 1
};

static OutputPin *AllPins[PIN_COUNT];
static OutputPin *PicoLed;  
/* Notes
   =====
   Une sortie GPIO est un composant gpio::GPIOBinaryOutput.

  */

ProfaluxBlind::ProfaluxBlind() {
  position = 0.5f;
}

void ProfaluxBlind::setPinOutput(int pinNumber, OutputPin *&thePin) {
  OutputPin *pin = NULL;
  if (pinNumber < PIN_COUNT) {
    pin = AllPins[pinNumber];
  }
  thePin = pin;
}
  
void ProfaluxBlind::setControler(ProfaluxControler *controler) {
  this->controler = controler;
}

void ProfaluxBlind::init_() {
  if (this->controler) {
    if (!initDone) {
      this->ledPin =  PicoLed; // TODO
      
      int upPinNumber = 0;
      int stopPinNumber = 0;
      int downPinNumber = 0;
      
      controler->getPinNumbers(blindNumber, upPinNumber, stopPinNumber, downPinNumber);
      ESP_LOGI(TAG, "Blind %d: %d %d %d", blindNumber, upPinNumber, stopPinNumber, downPinNumber); 
      setPinOutput(upPinNumber, this->upPin);
      setPinOutput(stopPinNumber, this->stopPin);
      setPinOutput(downPinNumber, this->downPin);
      initDone = true;
    }
  }
}

    
void BlinkTest(int start, int end) {
  for (int i=0;i<3;i++) {
    PicoLed->blink(100,200);
    for (int j=start; j<=end; j++) {
      if (j<PIN_COUNT && AllPins[j] != NULL) {
        AllPins[j]->turn_on();
      }
    }
    delay(100);
    for (int j=start; j<=end; j++) {
      if (j<PIN_COUNT && AllPins[j] != NULL) {
        AllPins[j]->turn_off();
      }
    }
  }
}

void ProfaluxBlind::setup() {
  // ESP_LOGI(TAG, "Debut Setup"); Trace ne fonctionne pas dans setup()

  PicoLed = new OutputPin(32);
  for (int i=0; i<PIN_COUNT;i++) {
    if (CanPin[i]) {
      AllPins[i] = new OutputPin(i);
      ESP_LOGD(TAG,"Pin[%d] = 0x%8x", i, (long) AllPins[i]);
    }
    else {
      AllPins[i] = NULL;
      ESP_LOGD(TAG,"Pas de pin %d");
    }
  }
  
  PicoLed->blink(200,200, 3);   
}

void ProfaluxBlind::setNumber(int blindNumber) {
  this->blindNumber = blindNumber;
}

void ProfaluxBlind::loop() {
}

cover::CoverTraits ProfaluxBlind::get_traits() {
  auto traits = cover::CoverTraits();
  //TODO
  traits.set_is_assumed_state(false);
  traits.set_supports_position(true);
  traits.set_supports_stop(true);
  traits.set_supports_tilt(false);

  return traits;
}

void ProfaluxBlind::control(const cover::CoverCall &call) {
  if (!initDone) {
    this->init_();
  }
  
  OutputPin *pin = NULL;
  PicoLed->blink(20);
  
  float pos = call.get_position().value();
  
  if (controler == NULL) {
    ESP_LOGW(TAG, "Remote %d: pas de controleur", blindNumber);
  }
  else {
    std::string ctrl_id = controler->get_controler_id();
    ESP_LOGD(TAG, "Remote %d: %s", blindNumber, ctrl_id.c_str());
  }
  
  if (call.get_stop()) {
    ESP_LOGI(TAG, "Remote %d STOP");
    pin = this->stopPin;
  }
  else {
    if (call.get_position() == cover::COVER_OPEN) {
      ESP_LOGI(TAG, "Remote %d UP %f", blindNumber, pos);
      pin = this->upPin;
    }
    if (call.get_position() == cover::COVER_CLOSED) {
      ESP_LOGI(TAG, "Remote %d DOWN %f", blindNumber, pos);
      pin = this->downPin;
    }
  }
  if (pin!=NULL) {
    ESP_LOGI(TAG, "Activation Pin %d", pin->getPinNumber());
    this->activateMotor(pin);
  }    
}

void ProfaluxBlind::stopAll(DelayedAction *todo) {
  if (upPin!=NULL) upPin->turn_off();
  if (stopPin!=NULL) stopPin->turn_off();
  if (downPin!=NULL) downPin->turn_off();
}

void ProfaluxBlind::activateMotor(OutputPin *pin) {
  pin->turn_on();
  DelayedAction::Callback_t pMethod = &ProfaluxBlind::stopAll;
  
  DelayedAction *off = new DelayedAction(this, millis()+1000, pMethod);
  controler->add_todo(off);
}

}  // namespace profalux_controler
}  // namespace esphome
