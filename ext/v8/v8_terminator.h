#include "v8.h"
#include "platform.h"

using namespace v8;

class TerminatorThread : public v8::internal::Thread {
 public:
  explicit TerminatorThread(int executing_thread_id,int timeout) : executing_thread_id_(executing_thread_id), timeout_(timeout), timedout_(false), finished_(false) {
    sem_ = internal::OS::CreateSemaphore(0);
  }
    ~TerminatorThread();
    virtual void Run();
    void Finished();
    bool Timedout() { return timedout_; }

 private:
    int executing_thread_id_;
    int timeout_; // microseconds ex: (3 * 1000000 = 3 seconds)
    bool timedout_;
    bool finished_;
    internal::Semaphore* sem_;
};
