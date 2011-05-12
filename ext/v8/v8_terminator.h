#ifndef _RR_V8_TERMINATOR_
#define _RR_V8_TERMINATOR_

#include "v8.h"
#include "platform.h"

using namespace v8;

class TerminatorThread : public internal::Thread {
 public:
  explicit TerminatorThread(int timeout) : timeout_(timeout), timedout_(false), finished_(false) {
    sem_ = internal::OS::CreateSemaphore(0);
  }
    ~TerminatorThread();
    virtual void Run();
    void Finished();
    bool Timedout() { return timedout_; }

 private:
    int timeout_; // microseconds ex: (3 * 1000000 = 3 seconds)
    bool timedout_;
    bool finished_;
    internal::Semaphore* sem_;
};

#endif
