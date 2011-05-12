#include "v8.h"
#include "v8_ref.h"
#include "platform.h"
#include "v8_script.h"
#include "v8_terminator.h"

using namespace v8;

void TerminatorThread::Run() {
  sem_->Wait(timeout_);

  if(finished_) {
    return;
  }

  timedout_ = true;
  {
	V8::TerminateExecution();
  }
}

void TerminatorThread::Finished() { 
  finished_ = true; 
  sem_->Signal();
}

TerminatorThread::~TerminatorThread() {
  delete sem_;
}

using namespace v8;

