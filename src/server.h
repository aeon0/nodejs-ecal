#pragma once
#include <iostream>
#include <napi.h>
#include <ecal/ecal.h>


// Example from docs: https://github.com/nodejs/node-addon-api/blob/main/doc/object_wrap.md
class Server : public Napi::ObjectWrap<Server> {
public:
  static void Init(Napi::Env env, Napi::Object& exports);

  Server(const Napi::CallbackInfo& info);
  void addMethodCallback(const Napi::CallbackInfo& info);
  void addEventCallback(const Napi::CallbackInfo& info);
  
private:
  int onMethod(const std::string& method, const std::string& reqType, const std::string& respType, const std::string& request, std::string& response);
  void onEvent(const char* name, const struct eCAL::SServerEventCallbackData* data);

  std::unique_ptr<eCAL::CServiceServer> _server;
  Napi::ThreadSafeFunction _tsFuncMethodCb;
  Napi::ThreadSafeFunction _tsFuncEventCb;
};
