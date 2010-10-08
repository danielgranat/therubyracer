#include "v8.h"
#include "platform.h"

using namespace v8;

class TerminatorThread : public v8::internal::Thread {
 public:
  explicit TerminatorThread(int executing_thread_id,int timeout) : executing_thread_id_(executing_thread_id), timeout_(timeout), timedout_(false) {}
    virtual void Run();
    int GetV8ThreadId() { return v8_thread_id_; }
    bool Timedout() { return timedout_; }

 private:
    int executing_thread_id_;
    int v8_thread_id_;
    int timeout_; // milliseconds
    bool timedout_;
};
