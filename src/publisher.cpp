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

void Publisher::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Publisher", {
    InstanceMethod<&Publisher::send>("send", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Publisher", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
