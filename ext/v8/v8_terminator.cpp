#include "v8.h"
#include "platform.h"
#include "v8_terminator.h"

using namespace v8;



void TerminatorThread::Run() {
{
	v8::Locker locker;
	v8_thread_id_ = v8::V8::GetCurrentThreadId();
}

v8::internal::OS::Sleep(timeout_);
printf("Terminating now...\n");
{
	v8::Locker locker;
	v8::V8::TerminateExecution(executing_thread_id_);
}
printf("Terminated\n");
}



void ExecutorThread::Run() {
  printf("Starting Executor  action....\n");
  v8::Locker locker;
  v8_thread_id_ = v8::V8::GetCurrentThreadId();
  printf("Threadid: %d\n",v8_thread_id_);


  result_ = script_->Run();
}
