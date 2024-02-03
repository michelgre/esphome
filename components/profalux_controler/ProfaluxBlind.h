#pragma once
#include <string>
#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"

#include "OutputPin.h"
#include "ScheduledTask.h"
#include "Task.h"
#include "ProfaluxControler.h"

namespace esphome {
namespace profalux_controler {

using namespace mr::outputpin;

class ProfaluxBlind;
class ProfaluxControler;
  
class ProfaluxBlind : public cover::Cover, public Component {
  
public:

  ProfaluxBlind(); 
  void setup() override;
  void loop() override;
  void set_entity_id(const std::string& entity_id) { entity_id_ = entity_id; }
  std::string get_entity_id() { return entity_id_; }
  esphome::cover::CoverTraits get_traits() override;
  void setControler(ProfaluxControler *controler);
  void setNumber(int blindNumber);

  void set_pin(const OutputPin *pin);
  
  virtual float get_setup_priority() const { return 0.2; }
  virtual void stop_control(ScheduledTask<ProfaluxBlind, OutputPin *> *task);
  void turn_on(Task<ProfaluxBlind, OutputPin *> *task);
protected:
  std::string entity_id_;
  ProfaluxControler *controler;
  int blindNumber = 1;
  OutputPin *upPin = NULL;
  OutputPin *downPin = NULL;
  OutputPin *stopPin = NULL;
  bool initDone = false;
 
  void init_();
  void control(const cover::CoverCall &call) override;
  void activateMotor(OutputPin *pin);
 
};

}  // namespace profalux_controler
}  // namespace esphome
