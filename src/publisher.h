#pragma once
#include <iostream>
#include <thread>
#include <napi.h>
#include <ecal/ecal.h>
#include <ecal/msg/publisher.h>

// Example from docs: https://github.com/nodejs/node-addon-api/blob/main/doc/object_wrap.md
class Publisher : public Napi::ObjectWrap<Publisher> {
  public:
    static void Init(Napi::Env env, Napi::Object& exports);

    Publisher(const Napi::CallbackInfo& info);
    void send(const Napi::CallbackInfo& info);
    void addEventCallback(const Napi::CallbackInfo& info);

  private:
    void onEvent(const char* topicName, const struct eCAL::SPubEventCallbackData* data);

    eCAL::CPublisher _pub;
    Napi::ThreadSafeFunction _tsFuncEventCb;
};
