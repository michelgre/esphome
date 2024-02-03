#pragma once

#include "esphome/core/datatypes.h"
#include "Task.h"

namespace esphome {
namespace profalux_controler {

class GenericScheduledTask : virtual public GenericTask {
public:
  
  struct CompareDesc {
      bool operator() (const GenericScheduledTask *a1, const GenericScheduledTask *a2) const {
          return a1->get_when()  > a2->get_when() ;
      }
  };

  GenericScheduledTask(uint32_t when) {  this->when = when; }
  inline uint32_t get_when() const { return when; }
protected:
  uint32_t when;
};


template<class T,class D>
class ScheduledTask : virtual public Task<T,D>, public GenericScheduledTask {
public: 
  ScheduledTask(const Task<T,D> task, uint32_t when) : Task<T,D>(task), GenericScheduledTask(when) {  };
  D get_data() { return get_data(); }
  void do_it() override { Task<T,D>::do_it(); }
protected:
};

}  // namespace profalux_controler
}  // namespace esphome

  