#include "v8.h"
#include "platform.h"
#include "v8_ref.h"
#include "v8_script.h"
#include "v8_terminator.h"

using namespace v8;

namespace {

  VALUE New(VALUE self, VALUE source, VALUE source_name) {
    HandleScope scope;
    Local<String> src(rr_rb2v8(source)->ToString());
    Local<String> src_name(rr_rb2v8(source_name)->ToString());
    return rr_v8_ref_create(self, Script::Compile(src, src_name));
  }

  VALUE Compile(VALUE self, VALUE source, VALUE source_name) {
    HandleScope scope;
    Local<String> src(rr_rb2v8(source)->ToString());
    Local<String> src_name(rr_rb2v8(source_name)->ToString());
    return rr_v8_ref_create(self, Script::Compile(src, src_name));
  }

  VALUE Run(VALUE self) {
    HandleScope scope;
    Local<Script> script(V8_Ref_Get<Script>(self));
    Local<Value> result(script->Run());
    return result.IsEmpty() ? Qnil : rr_v82rb(result);
  }


  VALUE RunTimeout(VALUE self,VALUE timeout) {
    {
      v8::Locker locker;
      v8::Locker::StartPreemption(1);
    }


    Local<Script> script(V8_Ref_Get<Script>(self));
    ExecutorThread executor(script);
    executor.Start();
     int t_id;
    printf("Started executor\n");
    v8::internal::OS::Sleep(2000);
    { 
       v8::Locker locker;
       t_id = executor.GetV8ThreadId();
    }

    printf("Got e thread: %d\n",t_id);

    TerminatorThread terminator(NUM2INT(timeout));
    terminator.Start(); 
    executor.Join();
    v8::V8::TerminateExecution(terminator.GetV8ThreadId());
    Local<Value> result(executor.result());
    v8::Locker::StopPreemption();
    return result.IsEmpty() ? Qnil : rr_v82rb(result);
    }

}

void rr_init_script() {
  VALUE ScriptClass = rr_define_class("Script");
  rr_define_singleton_method(ScriptClass, "New", New, 2);
  rr_define_singleton_method(ScriptClass, "Compile", Compile, 2);
  rr_define_method(ScriptClass, "Run", Run, 0);
  rr_define_method(ScriptClass, "RunTimeout", RunTimeout, 1);
}
