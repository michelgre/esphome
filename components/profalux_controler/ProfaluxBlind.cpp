#include "esphome/core/log.h"
#ifndef VSCODE
#include "esphome.h"
#endif

#include "ProfaluxBlind.h"
#include "ScheduledTask.h"
#include "OutputPin.h"

namespace esphome {
namespace profalux_controler {

using namespace api;
using namespace output;
using namespace mr::outputpin;

static const char* const TAG = "profalux-blind";

ProfaluxBlind::ProfaluxBlind() {
  position = 0.5f;
}

void ProfaluxBlind::setControler(ProfaluxControler *controler) {
  this->controler = controler;
}

void ProfaluxBlind::init_() {
  if (this->controler) {
    if (!initDone) {
      
      int upPinNumber = 0;
      int stopPinNumber = 0;
      int downPinNumber = 0;
      
      controler->get_pin_numbers(blindNumber, upPinNumber, stopPinNumber, downPinNumber);
      ESP_LOGI(TAG, "Blind %d: %d %d %d", blindNumber, upPinNumber, stopPinNumber, downPinNumber);
      this->upPin = controler->get_pin(upPinNumber);
      this->stopPin = controler->get_pin(stopPinNumber);
      this->downPin = controler->get_pin(downPinNumber);
      initDone = true;
    }
  }
}

void ProfaluxBlind::setup() {
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
  traits.set_supports_tilt(true);

  return traits;
}

void ProfaluxBlind::control(const cover::CoverCall &call) {
  if (!initDone) {
    this->init_();
  }
  OutputPin *pin = NULL;
  controler->blink(controler->get_led_pin(), 20);

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

void ProfaluxBlind::stop_control(ScheduledTask<ProfaluxBlind, OutputPin *> *todo) {
  if (todo!=NULL) {
    OutputPin *pin = todo->get_data();
    if (pin!=NULL) {
      pin->turn_off();
    }
  }
  else {
    if (upPin!=NULL) upPin->turn_off();
    if (stopPin!=NULL) stopPin->turn_off();
    if (downPin!=NULL) downPin->turn_off();
  }
}

void ProfaluxBlind::turn_on(Task<ProfaluxBlind, OutputPin *> *task) {
  controler->blink(task->get_data(), controler->get_signal_duration());
}

void ProfaluxBlind::activateMotor(OutputPin *pin) {
  controler->queue_blink(pin, controler->get_signal_duration());
}

}  // namespace profalux_controler
}  // namespace esphome
