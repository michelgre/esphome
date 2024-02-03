#pragma once

#include "esphome/core/datatypes.h"
#include "Task.h"

namespace esphome {
namespace profalux_controler {

class GenericQueuedTask : virtual public GenericTask {
public: 
  enum State {
    CREATED,
    STARTED,
    FINISHED
  };
  bool is_started() { return state == State::STARTED; }
  bool is_finished() { return state == State::FINISHED; }
  virtual void set_finished() { state = State::FINISHED; }
protected:
  State state;
};


template<class T,class D>
class QueuedTask : virtual public Task<T,D>, public GenericQueuedTask {
public: 

  QueuedTask(Task<T,D> task) : Task<T,D>(task) { state = State::CREATED; };

  void do_it() override { state = State::STARTED; Task<T,D>::do_it(); }
  void terminate() override { set_finished(); ESP_LOGD("queued-task", "terminate %d", get_id());}

};

}  // namespace profalux_controler
}  // namespace esphome

  