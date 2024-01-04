#include "DelayedAction.h"

namespace esphome {
namespace profalux_controler {

void DelayedAction::do_it() {
  (who->*what) (this);
}

}  // namespace profalux_controler
}  // namespace esphome

