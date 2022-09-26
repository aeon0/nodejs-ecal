#include "client.h"


Client::Client(const Napi::CallbackInfo& info) :
  Napi::ObjectWrap<Client>(info)
{
  Napi::String serviceName = info[0].As<Napi::String>();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Create Client: " << serviceName.ToString().Utf8Value() << std::endl;
#endif
  _client = std::make_unique<eCAL::CServiceClient>(serviceName);
}

void Client::callAsync(const Napi::CallbackInfo& info) {
  Napi::String methodName = info[0].As<Napi::String>();
  Napi::String request = info[1].As<Napi::String>();
  int timeout = static_cast<int>(info[2].As<Napi::Number>());

  _client->CallAsync(methodName, request, timeout);
}

void Client::addResponseCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Napi::Function napiFunction = info[0].As<Napi::Function>();
  _tsFuncResponseCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);

  using namespace std::placeholders;
  auto res = _client->AddResponseCallback(std::bind(&Client::onResponse, this, _1));
}

void Client::addEventCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  auto eventType = eCAL_Client_Event(static_cast<int>(info[0].As<Napi::Number>()));
  Napi::Function napiFunction = info[1].As<Napi::Function>();
  _tsFuncEventCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);

  using namespace std::placeholders;
  _client->AddEventCallback(eventType, std::bind(&Client::onEvent, this, _1, _2));
}

int Client::onResponse(const struct eCAL::SServiceResponse& serviceResponse) {
  _tsFuncResponseCb.Acquire();
  // Copy data because the jsCallback will not block onMethod()
  eCAL::SServiceResponse cp = serviceResponse;

  auto callback = [cp](Napi::Env env, Napi::Function jsCallback) {
    // Construct Napi::Object
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "call_state"), static_cast<int>(cp.call_state));
    obj.Set(Napi::String::New(env, "ret_state"), static_cast<int>(cp.ret_state));
    obj.Set(Napi::String::New(env, "error_msg"), cp.error_msg);
    obj.Set(Napi::String::New(env, "host_name"), cp.host_name);
    obj.Set(Napi::String::New(env, "method_name"), cp.method_name);
    obj.Set(Napi::String::New(env, "response"), cp.response);
    obj.Set(Napi::String::New(env, "service_id"), cp.service_id);
    obj.Set(Napi::String::New(env, "service_name"), cp.service_name);

    jsCallback.Call({ obj });
  };

  napi_status status = _tsFuncResponseCb.BlockingCall(callback);
  _tsFuncResponseCb.Release();
  return 0; // What should be returned here?
}

void Client::onEvent(const char* name, const struct eCAL::SClientEventCallbackData* data) {
  _tsFuncEventCb.Acquire();
  std::string cpName(name);
  auto cpData = *data;

  auto callback = [cpName, cpData](Napi::Env env, Napi::Function jsCallback) {
    // Construct Napi::Object
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "type"), static_cast<int>(cpData.type));
    obj.Set(Napi::String::New(env, "time"), cpData.time);

    jsCallback.Call({
      Napi::String::New(env, cpName),
      obj
      });
  };
  napi_status status = _tsFuncEventCb.BlockingCall(callback);
  _tsFuncEventCb.Release();
}

void Client::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Client", {
    InstanceMethod<&Client::addResponseCallback>("addResponseCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&Client::addEventCallback>("addEventCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&Client::callAsync>("callAsync", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Client", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
