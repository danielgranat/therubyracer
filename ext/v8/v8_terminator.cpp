#include "v8.h"
#include "v8_ref.h"
#include "platform.h"
#include "v8_terminator.h"

using namespace v8;

void TerminatorThread::Run()
{
  v8::Locker locker;
  v8_thread_id_ = v8::V8::GetCurrentThreadId();

  v8::internal::OS::Sleep(timeout_);
  {
    v8::Locker locker;
    timedout_ = true;
    v8::V8::TerminateExecution(executing_thread_id_);
  }
}

