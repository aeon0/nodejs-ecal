#include "publisher.h"


Publisher::Publisher(const Napi::CallbackInfo& info) :
  Napi::ObjectWrap<Publisher>(info)
{
  Napi::String topic = info[0].As<Napi::String>();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Create Publisher: " << topic.ToString().Utf8Value() << std::endl;
#endif
  auto ecalNodeName = "Nodejs-eCAL Publisher: " + topic.ToString().Utf8Value();
  eCAL::Initialize(0, 0, ecalNodeName.c_str());
  _pub = eCAL::CPublisher(topic);
}

void Publisher::send(const Napi::CallbackInfo& info) {
  auto data = info[0].As<Napi::ArrayBuffer>();
  void* buf = data.Data();
  size_t size = data.ByteLength();
  auto ts = info[1].As<Napi::Number>();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Sending data: " << data.ToString().Utf8Value() << std::endl;
#endif
  _pub.Send(buf, size, ts.Int64Value());
}

void Publisher::addEventCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function napiFunction = info[1].As<Napi::Function>();
  auto eventType = eCAL_Publisher_Event(static_cast<int>(info[0].As<Napi::Number>()));
  _tsFuncEventCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);
  _pub.AddEventCallback(eventType, std::bind(&Publisher::onEvent, this, std::placeholders::_1, std::placeholders::_2));
}

void Publisher::onEvent(const char* topicName, const eCAL::SPubEventCallbackData* data) {
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

void Publisher::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Publisher", {
    InstanceMethod<&Publisher::send>("send", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&Publisher::addEventCallback>("addEventCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Publisher", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
