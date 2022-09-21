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
  _tsFuncReceiveCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);
  _sub.AddReceiveCallback(std::bind(&Subscriber::onReceive, this, std::placeholders::_1, std::placeholders::_2));
}

void Subscriber::addEventCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function napiFunction = info[1].As<Napi::Function>();
  auto eventType = eCAL_Subscriber_Event(static_cast<int>(info[0].As<Napi::Number>()));
  _tsFuncEventCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);
  _sub.AddEventCallback(eventType, std::bind(&Subscriber::onEvent, this, std::placeholders::_1, std::placeholders::_2));
}

void Subscriber::onReceive(const char* topicName, const eCAL::SReceiveCallbackData* data) {
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Receive data from " << topicName << ": " << data << std::endl;
#endif
  _tsFuncReceiveCb.Acquire();
  auto buf = data->buf;
  auto size = data->size;

  std::string topic(topicName);
  void* copied_buf = malloc(size);
  memcpy(copied_buf, buf, size);

  auto callback = [topic, copied_buf, size](Napi::Env env, Napi::Function jsCallback) {
    jsCallback.Call({Napi::String::New(env, topic), Napi::ArrayBuffer::New(env, copied_buf, size)});
    free(copied_buf);
  };
  napi_status status = _tsFuncReceiveCb.BlockingCall(callback);
  _tsFuncReceiveCb.Release();
}

void Subscriber::onEvent(const char* topicName, const eCAL::SSubEventCallbackData* data) {
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Receive event from " << topicName << ": " << data->type << std::endl;
#endif
  _tsFuncEventCb.Acquire();
  std::string topic(topicName);
  auto cpData = *data;

  auto callback = [topic, cpData](Napi::Env env, Napi::Function jsCallback) {
    // Construct Napi::Object
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "type"), static_cast<int>(cpData.type));
    obj.Set(Napi::String::New(env, "time"), cpData.time);
    obj.Set(Napi::String::New(env, "clock"), cpData.clock);

    jsCallback.Call({Napi::String::New(env, topic), obj});
  };
  napi_status status = _tsFuncEventCb.BlockingCall(callback);
  _tsFuncEventCb.Release();
}

void Subscriber::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Subscriber", {
    InstanceMethod<&Subscriber::addReceiveCallback>("addReceiveCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&Subscriber::addEventCallback>("addEventCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Subscriber", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
