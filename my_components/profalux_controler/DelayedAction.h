#pragma once

#include "esphome/core/datatypes.h"

namespace esphome {
namespace profalux_controler {

class GenericDelayedAction {
public:
  
  struct CompareDesc {
      bool operator() (const GenericDelayedAction *a1, const GenericDelayedAction *a2) const {
          return a1->get_when()  > a2->get_when() ;
      }
  };

  GenericDelayedAction(uint32_t when) {  this->when = when; }
  virtual void do_it() = 0;
  inline uint32_t get_when() const { return when; }
protected:
  uint32_t when;
};


template<class T,class D>
class DelayedAction : public GenericDelayedAction {
public: 
  typedef void (T::*Callback_t)(DelayedAction<T,D> *);
 
  DelayedAction(T *who, uint32_t when, Callback_t what, D data) : GenericDelayedAction(when) { this->who = who; this->what = what; this->data = data; }
  void do_it() {
    (who->*what) (this);
  }
  D get_data() { return data; }
  
protected:
  T *who;
  Callback_t what;
  D data;
};

}  // namespace profalux_controler
}  // namespace esphome

  