#pragma once
#include <iostream>
#include <napi.h>
#include <ecal/ecal.h>


// Example from docs: https://github.com/nodejs/node-addon-api/blob/main/doc/object_wrap.md
class Client : public Napi::ObjectWrap<Client> {
public:
  static void Init(Napi::Env env, Napi::Object& exports);

  Client(const Napi::CallbackInfo& info);
  void callAsync(const Napi::CallbackInfo& info);

  void addResponseCallback(const Napi::CallbackInfo& info);
  void addEventCallback(const Napi::CallbackInfo& info);

private:
  int onResponse(const struct eCAL::SServiceResponse& serviceResponse);
  void onEvent(const char* name, const struct eCAL::SClientEventCallbackData* data);

  std::unique_ptr<eCAL::CServiceClient> _client;
  Napi::ThreadSafeFunction _tsFuncResponseCb;
  Napi::ThreadSafeFunction _tsFuncEventCb;
};
