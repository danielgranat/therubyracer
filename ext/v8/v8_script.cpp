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
	printf( "Running (Actualy) ...\n");
	HandleScope scope;
    Local<Script> script(V8_Ref_Get<Script>(self));
    Local<Value> result(script->Run());
    return result.IsEmpty() ? Qnil : rr_v82rb(result);
  }

  VALUE RunTimeout(VALUE self, VALUE timeout, VALUE source, VALUE source_name ) {
	printf( "Running ...\n");
	V8::AssignThreadId();
	printf( "Running thread id(%d)\n", V8::GetCurrentThreadId() );

	printf( "Running thread lock(%d)\n", V8::GetCurrentThreadId() );
	TerminatorThread terminator(V8::GetCurrentThreadId(), NUM2INT(rb_to_int(timeout)));
	terminator.Start();

    VALUE result = Run( self );
    printf( "Done Running thread id(%d)\n", V8::GetCurrentThreadId() );
    terminator.Finished();
    terminator.Join();

    if(terminator.Timedout()) {
      result = Qnil;
      rb_raise(rb_eScriptError, "Script execusion has timed out");
    }

    return result;
  }
}

void rr_init_script() {
  VALUE ScriptClass = rr_define_class("Script");
  rr_define_singleton_method(ScriptClass, "New", New, 2);
  rr_define_singleton_method(ScriptClass, "Compile", Compile, 2);
  rr_define_method(ScriptClass, "Run", Run, 0);
  rr_define_method(ScriptClass, "Run", RunTimeout, 3);
}
