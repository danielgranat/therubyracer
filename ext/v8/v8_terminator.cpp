#include "v8.h"
#include "v8_ref.h"
#include "platform.h"
#include "v8_script.h"
#include "v8_terminator.h"

using namespace v8;

class ExecutorThread : public internal::Thread {
public:
  explicit ExecutorThread(VALUE context, VALUE source, VALUE source_name, int timeout) : context_(context), source_(source), source_name_(source_name), timeout_(timeout), timedout_(false) {}

  virtual void Run() {
    Locker locker;
    
    TerminatorThread terminator(V8::GetCurrentThreadId(), timeout_);
    terminator.Start();

    Persistent<Context> context(V8_Ref_Get<Context>(context_));

    context->Enter();

    Handle<String> src(rr_rb2v8(source_)->ToString());
    Handle<String> src_name(rr_rb2v8(source_name_)->ToString());
    Handle<Script> script = Script::Compile(src, src_name);

    result_ = script->Run();

    context->Exit();

    terminator.Finished();
    terminator.Join();
    timedout_ = terminator.Timedout();
  }

  Handle<Value> Result() {
    return result_;
  }

  bool Timedout() {
    return timedout_;
  }

  TryCatch Exception() {
    return try_catch_;
  }

  bool HasError() {
    return try_catch_.HasCaught();
  }

private:
  VALUE context_;
  VALUE source_;
  VALUE source_name_;

  Handle<Value> result_;
  TryCatch try_catch_;
  int timeout_;
  bool timedout_;
};


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

  /*
    In context.rb, replace entire eval C::Locker TryCatch Block with
    result = C::Terminator::Exec(@timeout, @native, @to.v8(javascript.to_s), @to.v8(filename.to_s))
   */
  VALUE Exec(VALUE self, VALUE timeout, VALUE context, VALUE source, VALUE source_name) {
    Unlocker unlocker;

    ExecutorThread executor(context, source, source_name, NUM2INT(rb_to_int(timeout)));
    executor.Start();
    executor.Join();

    if(executor.Timedout()) {
      return TimeoutErrorClass;
    } else if (executor.HasError()) {
      // Need to return error here
    }

    VALUE result;
    {
      Locker locker;
      result = executor.Result().IsEmpty() ? Qnil : rr_v82rb(executor.Result());
    }
    return result;
  }
}

void rr_init_terminator() {
  TimeoutErrorClass = rr_define_class("TimeoutError", rb_eException);
  VALUE TerminatorClass = rr_define_class("Terminator");
  rr_define_singleton_method(TerminatorClass, "Run", Run, 2);
  rr_define_singleton_method(TerminatorClass, "Exec", Exec, 4);
}
