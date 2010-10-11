#include "v8.h"
#include "platform.h"

using namespace v8;

class TerminatorThread : public v8::internal::Thread {
 public:
  explicit TerminatorThread(int executing_thread_id,int timeout) : executing_thread_id_(executing_thread_id), timeout_(timeout), timedout_(false), finished_(false) {}
    virtual void Run();
    bool Timedout() { return timedout_; }
    void Finished() { finished_ = true; }

 private:
    int executing_thread_id_;
    int timeout_; // milliseconds
    bool timedout_;
    bool finished_;
};
