#pragma once

namespace esphome {
namespace profalux_controler {

class GenericTask {
public:
  virtual void do_it() = 0;
  virtual void terminate() {}
  int get_id() const { return taskId; }
protected:
  static int counter;
  int taskId = counter++;
};

template<class T,class D>
class Task : virtual public GenericTask {
public: 
  typedef void (T::*Callback_t)(Task<T,D> *);
  Task(const Task &task) {
    this->who = task.who; this->what = task.what; this->data = task.data; 
  }
  Task(T *who, Callback_t what, D data) { this->who = who; this->what = what; this->data = data; }
  void do_it() override {
    (who->*what) (this);
  }
  D get_data() const { return data; }

protected:
  T *who;
  Callback_t what;
  D data;
};

}  // namespace profalux_controler
}  // namespace esphome
