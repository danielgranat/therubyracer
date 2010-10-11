#include "v8.h"
#include "v8_ref.h"
#include "platform.h"
#include "v8_terminator.h"

using namespace v8;

void TerminatorThread::Run()
{
  const int SLEEP_FOR = 10;

  internal::OS::Sleep(1);

  for(int sleep=SLEEP_FOR; sleep<timeout_; sleep+=SLEEP_FOR) {
    if(finished_) {
      return;
    }
    internal::OS::Sleep(SLEEP_FOR);
  }

  if(finished_) {
    return;
  }

  timedout_ = true;
  {
    Locker locker;
    V8::TerminateExecution(executing_thread_id_);
  }
}

