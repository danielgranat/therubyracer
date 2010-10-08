#include "v8.h"
#include "platform.h"

using namespace v8;


class TerminatorThread : public v8::internal::Thread {
  public:
    explicit TerminatorThread(int executing_thread_id,int timeout) : executing_thread_id_(executing_thread_id), timeout_(timeout) {}
    virtual void Run();
    int GetV8ThreadId() { return v8_thread_id_; }

  private:
    int executing_thread_id_;
    int v8_thread_id_;
    int timeout_;

};


class ExecutorThread : public v8::internal::Thread {
  public:
    explicit ExecutorThread(Local<Script> script) : script_(script) {}
    virtual void Run();
    int GetV8ThreadId() { return v8_thread_id_; }

    Local<Value> result() { return result_; }
  private:
    int v8_thread_id_;
    Local<Value> result_;
    Local<Script> script_;

};


