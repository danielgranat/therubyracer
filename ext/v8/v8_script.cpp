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
    Locker locker;
    TerminatorThread terminator(V8::GetCurrentThreadId(), NUM2INT(rb_to_int(timeout)));
    terminator.Start();

    VALUE result = Run(self);

    terminator.Finished();
    terminator.Join();

    if(terminator.Timedout()) {
      // would like to throw a timeout error
    }

    return result;
  }

  VALUE SetConstraints(VALUE self, VALUE young_size, VALUE old_size) {
    static const int K = 1024;
    ResourceConstraints constraints;
    constraints.set_max_young_space_size(NUM2INT(rb_to_int(young_size)) * K);
    constraints.set_max_old_space_size(NUM2INT(rb_to_int(old_size)) * K);
    return rr_v82rb(SetResourceConstraints(&constraints));
  }
}

void rr_init_script() {
  VALUE ScriptClass = rr_define_class("Script");
  rr_define_singleton_method(ScriptClass, "New", New, 2);
  rr_define_singleton_method(ScriptClass, "Compile", Compile, 2);
  rr_define_singleton_method(ScriptClass, "SetConstraints", SetConstraints, 2);
  rr_define_method(ScriptClass, "Run", Run, 0);
  rr_define_method(ScriptClass, "RunTimeout", RunTimeout, 1);
}
