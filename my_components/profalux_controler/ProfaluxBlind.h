#pragma once
#include <string>
#include "esphome/core/component.h"
#include "esphome/components/cover/cover.h"

#include "OutputPin.h"
#include "DelayedAction.h"
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
  cover::CoverTraits get_traits() override;
  void setControler(ProfaluxControler *controler);
  void setNumber(int blindNumber);
  
  virtual float get_setup_priority() const { return 0.2; }
  virtual void stopAll(DelayedAction<ProfaluxBlind, OutputPin *> *todo);
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
