#include "esphome/core/log.h"

#include "ProfaluxControler.h"
#include "ScheduledTask.h"
#include "QueuedTask.h"
#include "Task.h"
#include "OutputPin.h"
#include "debug.h"

using namespace esphome::mr::outputpin;

namespace esphome {
namespace profalux_controler {

static const char* const TAG = "profalux-controler";

#define PIN_COUNT 29

  static int CanPin[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 0, 0, 0, 1, 1, 1
};

static OutputPin *AllPins[PIN_COUNT];
static OutputPin *PicoLed;  

void ProfaluxControler::setup() {
  PicoLed = new OutputPin(32);  // TODO: get # from platform
  for (int i=0; i<PIN_COUNT;i++) {
    if (CanPin[i]) {
      AllPins[i] = new OutputPin(i);
    }
    else {
      AllPins[i] = NULL;
    }
  }
  //this->blink(PicoLed, 30, 100, 3);
  ESP_LOGCONFIG(TAG, "Fin Setup\n");
}

OutputPin *ProfaluxControler::get_pin(int pinNumber) {
  if (pinNumber < PIN_COUNT) {
    return AllPins[pinNumber];
  }
  else {
    return NULL;
  }
}

void ProfaluxControler::trace_schedule() {
  Schedule pq (schedule);
  while (pq.size()>0) {
    GenericScheduledTask *task = pq.top();
    pq.pop();
    ESP_LOGD(TAG, "Schedule: %ld", task->get_when());
  }
}

int numLoop = 0;
int lastScheduledSeen = 0;
int lastQueuedSeen = 0;
void ProfaluxControler::loop() {
  // Execute scheduled tasks if they are due
  if (schedule.size()>0) {
    while (schedule.size()>0) {
      GenericScheduledTask *todo = schedule.top();
      if (lastScheduledSeen!=todo->get_id()) {
        ESP_LOGD(TAG,"loop: todo %d, first=%d, clock=%ld, time=%ld",schedule.size(), todo->get_id(), millis(), todo->get_when());
        lastScheduledSeen = todo->get_id();
      }
      if (todo->get_when()<=millis()) {
        schedule.pop();
        ESP_LOGD(TAG,"loop: doing %d, clock=%ld",todo->get_id(), millis());
        todo->do_it();
        delete todo;
      }
      else {
        break; // Plus rien à faire pour l'instant
      }
    }
  }

  // Execute / Check task queue
  if (taskQueue.size()>0) {
    GenericQueuedTask *currentTask = taskQueue.front();
    if (lastQueuedSeen!=currentTask->get_id()) {
      ESP_LOGD(TAG,"queued: %d, first=%d", taskQueue.size(), currentTask->get_id());
      lastQueuedSeen = currentTask->get_id();
    }

    if (currentTask->is_finished()) {
      ESP_LOGD(TAG,"clean task %d", currentTask->get_id());
      // The task has completed, remove it
      taskQueue.pop_front();
      delete currentTask;
    }
    else if (!currentTask->is_started()) {
      // Not yet started, start it
      ESP_LOGD(TAG,"start task %d", currentTask->get_id());
      currentTask->do_it();
    }
    else {
      // Nothing to do in this controler loop
    }
  }
  
}

void ProfaluxControler::register_blind(ProfaluxBlind *blind) {
  ESP_LOGI(TAG, "Register blind %x", blind);
  this->blinds_.push_back(blind);
  blind->setControler(this);
  
}

OutputPin *ProfaluxControler::get_led_pin() {
  return PicoLed;
}

void ProfaluxControler::get_pin_numbers(int blindNumber, int &upNumber, int &stopNumber, int &downNumber) {
  int n = 3 * (blindNumber - 1);
  if (n+2 < (sizeof(pins) / sizeof(int))) {
    upNumber = pins[n];
    stopNumber = pins[n+1];
    downNumber = pins[n+2];
  }
  else {
    // TODO
  }
}

void ProfaluxControler::turn_off(Task<ProfaluxControler, OutputPin *> *task) {
  task->get_data()->turn_off();
}

void ProfaluxControler::blink_again(Task<ProfaluxControler, BlinkData *> *task) {
  BlinkData *data = task->get_data() ;
  ESP_LOGD(TAG,"blink_again: task %d, on=%d, count=%d",task->get_id(), data->isOn, data->count);
  if (data->mainTask==NULL) {
    data->mainTask = task;
  }
  if (!data->isOn) {
    data->pin->turn_on();
    data->isOn = true;

    Task<ProfaluxControler, BlinkData *> nextBlinkTask(this, &ProfaluxControler::blink_again, data);
    this->schedule_task(nextBlinkTask, millis()+data->ms1);
  }
  else {
    data->pin->turn_off();
    data->isOn = false;
    data->count--;
    task->terminate();
    if (data->count==0) { // That's the end of this blink task
      if (data->mainTask!=NULL) {
        data->mainTask->terminate(); // Terminate the queued task, if any
      }
      delete data;
    }
    else {
     Task<ProfaluxControler, BlinkData *> nextBlinkTask(this, &ProfaluxControler::blink_again, data);
     this->schedule_task(nextBlinkTask, millis()+data->ms2);
    }
  }
}
  
void ProfaluxControler::blink(OutputPin *pin, int ms) {
  if (pin!=NULL) {
    pin->turn_on();
    Task<ProfaluxControler, OutputPin *> stopTask(this, &ProfaluxControler::turn_off, pin);
    this->schedule_task(stopTask, millis()+ms);
  }
}  

void ProfaluxControler::blink(OutputPin *pin, int ms1, int ms2, int count) {
  if (pin != NULL) {
    pin->turn_on();
    BlinkData *data = new BlinkData(pin, ms1, ms2, count, true);
    Task<ProfaluxControler, BlinkData *> offTask (this, &ProfaluxControler::blink_again, data);
    schedule_task(offTask, millis()+ms1);
  }
}

void ProfaluxControler::queue_blink(OutputPin *pin, int ms) {
  trace_free_heap(TAG);
  BlinkData *data = new BlinkData(pin, ms, 0, 1, false);

  // create a task
  Task<ProfaluxControler, BlinkData *> task(this, &ProfaluxControler::blink_again, data);
  queue_task(task);

}

const char* ProfaluxControler::get_tag() {
  return TAG;
}

}  // namespace profalux_controler
}  // namespace esphome
