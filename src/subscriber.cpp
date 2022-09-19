#include "subscriber.h"


Subscriber::Subscriber(const Napi::CallbackInfo& info) :
  Napi::ObjectWrap<Subscriber>(info)
{
  Napi::String topic = info[0].As<Napi::String>();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Create Subscriber: " << topic.ToString().Utf8Value() << std::endl;
#endif
  _sub = eCAL::CSubscriber(topic);
}

void Subscriber::addReceiveCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function napiFunction = info[0].As<Napi::Function>();
  _tsFunc = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);
  _sub.AddReceiveCallback(std::bind(&Subscriber::onReceive, this, std::placeholders::_2));
}

void Subscriber::onReceive(const eCAL::SReceiveCallbackData* data) {
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Receive data: " << data << std::endl;
#endif
  _tsFunc.Acquire();
  auto buf = data->buf;
  auto size = data->size;

  void* copied_buf = malloc(size);
  memcpy(copied_buf, buf, size);

  auto callback = [copied_buf, size](Napi::Env env, Napi::Function jsCallback) {
    jsCallback.Call({Napi::ArrayBuffer::New(env, copied_buf, size)});
    free(copied_buf);
  };
  napi_status status = _tsFunc.BlockingCall(callback);
  _tsFunc.Release();
}

void Subscriber::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Subscriber", {
    InstanceMethod<&Subscriber::addReceiveCallback>("addReceiveCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Subscriber", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
