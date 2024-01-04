#pragma once

#include <ctime>

#include "ProfaluxBlind.h"

namespace esphome {
namespace profalux_controler {


class DelayedAction {
public: 
  typedef void (ProfaluxBlind::*Callback_t)(DelayedAction *);
 
  DelayedAction(ProfaluxBlind *who, clock_t when, Callback_t what) { this->who = who; this->when = when; this->what = what; }
  void do_it();
  friend bool operator< (const DelayedAction &a1, const DelayedAction &a2) {
    return a2.when > a1.when;
  }
  inline clock_t get_when() { return when; }
  
protected:
  clock_t when;
  ProfaluxBlind *who;
  Callback_t what;
};

}  // namespace profalux_controler
}  // namespace esphome

  