
#include <napi.h>
#include <string>
#include <ecal/ecal.h>
#include "subscriber.h"
#include "publisher.h"
#include "client.h"
#include "server.h"


Napi::Boolean ok(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), eCAL::Ok());
}

void finalize(const Napi::CallbackInfo& info) {
  eCAL::Finalize();
}

void initialize(const Napi::CallbackInfo& info) {
  // std::string name = (std::string) info[0].ToString();
#if SHOW_DEBUG_COMMENTS
  std::cout << "[DEBUG] Init eCAL" << std::endl;
#endif
  eCAL::Initialize(0, 0, "Nodejs-eCAL");
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Subscriber::Init(env, exports);
  Publisher::Init(env, exports);
  Client::Init(env, exports);
  Server::Init(env, exports);

  exports.Set("ok", Napi::Function::New(env, ok));
  exports.Set("finalize", Napi::Function::New(env, finalize));
  exports.Set("initialize", Napi::Function::New(env, initialize));

  return exports;
}

NODE_API_MODULE(nodejsEcal, Init)
