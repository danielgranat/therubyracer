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
    Locker locker;
    V8::TerminateExecution(executing_thread_id_);
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

namespace {
  VALUE TimeoutErrorClass;

  VALUE Run(VALUE self, VALUE timeout, VALUE ref) {
    Locker locker;

    TerminatorThread terminator(V8::GetCurrentThreadId(), NUM2INT(rb_to_int(timeout)));
    terminator.Start();

    VALUE result = Qnil;
    if(ref && ref != Qnil) {
      HandleScope scope;
      Local<Script> script(V8_Ref_Get<Script>(ref));
      Local<Value> value(script->Run());
      if(value.IsEmpty() == false) {
        result = rr_v82rb(value);
      }
    } else {
      rb_yield(Qnil);
    }

    terminator.Finished();
    terminator.Join();

    if(terminator.Timedout()) {
      result = TimeoutErrorClass;
    }

    return result;
  }
}

void rr_init_terminator() {
  TimeoutErrorClass = rr_define_class("TimeoutError", rb_eException);
  VALUE TerminatorClass = rr_define_class("Terminator");
  rr_define_singleton_method(TerminatorClass, "Run", Run, 2);
}
