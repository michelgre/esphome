#pragma once
#include <string>
#include <queue>
#include <list>

#include "esphome/core/component.h"
#include "OutputPin.h"
#include "Task.h"
#include "ScheduledTask.h"
#include "QueuedTask.h"

#include "ProfaluxBlind.h"

using namespace esphome::mr::outputpin;

namespace esphome {
namespace profalux_controler {

class ProfaluxBlind;
class ProfaluxControler;
class BlinkData;
  
struct BlinkData {
  BlinkData(OutputPin *pin, int ms1, int ms2, int count, bool isOn) {
    this->pin = pin;
    this->ms1 = ms1;
    this->ms2 = ms2;
    this->count = count;
    this->isOn = isOn;
  }
  OutputPin *pin;
  int ms1;
  int ms2;
  int count;
  bool isOn;
  GenericTask *mainTask = NULL;
};

class ProfaluxControler : public Component {
public:
  typedef std::priority_queue<GenericScheduledTask *, std::vector<GenericScheduledTask *>, GenericScheduledTask::CompareDesc> Schedule;

  // Remote control and pins management
  void register_blind(ProfaluxBlind *blind);
  OutputPin *get_led_pin();
  void get_pin_numbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber);
  OutputPin *get_pin(int pinNumber);
  void queue_blink(OutputPin *pin, int ms);
  void blink(OutputPin *pin, int ms);
  void blink(OutputPin *pin, int ms1, int ms2, int count);
 
  // ESPHome main methods
  void setup() override;
  void loop() override;
 
  // Task management
  template<class T,class D>
  void schedule_task(const Task<T,D> &task, uint32_t when) {
    ScheduledTask<T,D> *sd = new ScheduledTask<T,D>(task, when);
    schedule.push(sd);
  }

  template<class T,class D>
  void queue_task(const Task<T,D> &task) {
    QueuedTask<T,D> *qt = new QueuedTask<T,D>(task);
    ESP_LOGD(get_tag(), "queue task %d->%d, current %d", task.get_id(), qt->get_id(),taskQueue.size());
    taskQueue.push_back(qt);
  }

  void trace_schedule();

  // ESPHome properties
  virtual float get_setup_priority() const { return 0.1; }
  void set_controler_id(std::string s) { controlerId = s; }
  std::string get_controler_id() { return controlerId; }
  int get_signal_duration() { return signalDuration; }
  void set_signal_duration(int signalDuration) { this->signalDuration = signalDuration; }

protected:
  void turn_off(Task<ProfaluxControler, OutputPin *> *task);
  void blink_again(Task<ProfaluxControler, BlinkData *> *task);
  //void do_queued_blink(Task<ProfaluxControler, BlinkData *> *task);

  std::string controlerId;   
  std::vector<ProfaluxBlind *> blinds_;
  int pins[18] = { 
    10, 11, 12, 
    7, 8, 9, 
    4, 5, 6, 
    27, 26, 22, 
    21, 20, 19,
    18, 17, 16    
  };
  int signalDuration = 1000;
  Schedule schedule;
  std::list<GenericQueuedTask *> taskQueue;
  static const char* get_tag();
};

}  // namespace profalux_controler
}  // namespace esphome
