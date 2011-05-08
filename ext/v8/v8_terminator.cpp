#include "v8.h"
#include "v8_ref.h"
#include "platform.h"
#include "v8_script.h"
#include "v8_terminator.h"

using namespace v8;

void TerminatorThread::Run() {
  V8::AssignThreadId();
  printf( "TerminatorThread thread id(%d), timeout(%d)\n", V8::GetCurrentThreadId(), timeout_ );
  if (! sem_->Wait(timeout_) )
	  printf( "TerminatorThread done waiting- timedout\n");
  else
	  printf( "TerminatorThread done waiting\n");

  if(finished_) {
    return;
  }

  timedout_ = true;
  {
	printf( "TerminatorThread lock.... \n");
    Locker locker;
    printf( "TerminatorThread locked! terminate(%d)\n", executing_thread_id_ );
    V8::TerminateExecution(executing_thread_id_);
    printf( "TerminatorThread terminated..!\n");
  }
}

void TerminatorThread::Finished() { 
  printf( "TerminatorThread Finished\n");
  finished_ = true; 
  sem_->Signal();
}

TerminatorThread::~TerminatorThread() {
  delete sem_;
}

using namespace v8;

