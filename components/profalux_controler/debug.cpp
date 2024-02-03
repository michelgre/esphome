#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/macros.h"
#include "esphome/core/helpers.h"

#include <Arduino.h>

#include "debug.h"

namespace esphome {
namespace profalux_controler {

void trace_free_heap(const char *TAG) {
  uint32_t fm = rp2040.getFreeHeap();
  ESP_LOGD(TAG, "Free heap: %d", fm);
}

}
}
