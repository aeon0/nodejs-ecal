#include "server.h"
#include <chrono>
#include <thread>


Server::Server(const Napi::CallbackInfo& info) :
  Napi::ObjectWrap<Server>(info)
{
  Napi::String serviceName = info[0].As<Napi::String>();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Create Server: " << serviceName.ToString().Utf8Value() << std::endl;
#endif
  _server = std::make_unique<eCAL::CServiceServer>(serviceName);
}

void Server::addMethodCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  std::string method = info[0].As<Napi::String>();
  std::string reqType = info[1].As<Napi::String>();
  std::string respType = info[2].As<Napi::String>(); 
  Napi::Function napiFunction = info[3].As<Napi::Function>();
  _tsFuncMethodCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);

  using namespace std::placeholders;
  _server->AddMethodCallback(method, reqType, respType, std::bind(&Server::onMethod, this, _1, _2, _3, _4, _5));
}

void Server::addEventCallback(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  auto eventType = eCAL_Server_Event(static_cast<int>(info[0].As<Napi::Number>()));
  Napi::Function napiFunction = info[1].As<Napi::Function>();
  _tsFuncEventCb = Napi::ThreadSafeFunction::New(env, napiFunction, "Callback", 0, 1);

  using namespace std::placeholders;
  _server->AddEventCallback(eventType, std::bind(&Server::onEvent, this, _1, _2));
}

int Server::onMethod(const std::string& method, const std::string& reqType, const std::string& respType, const std::string& request, std::string& response) {
  _tsFuncMethodCb.Acquire();

  std::string tmpResponse = "";
  bool jsCallbackDone = false;

  auto callback = [method, reqType, respType, request, &tmpResponse, &jsCallbackDone](Napi::Env env, Napi::Function jsCallback) {
    Napi::Value jsResponse = jsCallback.Call({ 
      Napi::String::New(env, method),
      Napi::String::New(env, reqType),
      Napi::String::New(env, respType),
      Napi::String::New(env, request)
    });
    tmpResponse = static_cast<std::string>(jsResponse.ToString());
    jsCallbackDone = true;
  };

  napi_status status = _tsFuncMethodCb.BlockingCall(callback);
  // Kind of hacky. The jsCallback is called in a new thread. But we have to wait
  // for a response to be able to fill the response ref
  // TODO: Is there an napi function type which will just block without having issues with
  //       threads? When calling the napi::function directly v8 runs into memory/scope/whatever issues
  bool success = true;
  auto start = std::chrono::high_resolution_clock::now();
  while (!jsCallbackDone) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    auto current = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();
    if (dur > 10) {
      success = false;
      break;
    }
  }
  _tsFuncMethodCb.Release();
  if (!success) return false;
  response = tmpResponse;
  return true;
}

void Server::onEvent(const char* name, const struct eCAL::SServerEventCallbackData* data) {
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

void Server::Init(Napi::Env env, Napi::Object& exports) {
  Napi::Function func = DefineClass(env, "Server", {
    InstanceMethod<&Server::addMethodCallback>("addMethodCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    InstanceMethod<&Server::addEventCallback>("addEventCallback", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
  });
  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  exports.Set("Server", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
}
