#pragma once
#include <iostream>
#include <napi.h>
#include <ecal/ecal.h>
#include <ecal/msg/subscriber.h>


// Example from docs: https://github.com/nodejs/node-addon-api/blob/main/doc/object_wrap.md
class Subscriber : public Napi::ObjectWrap<Subscriber> {
  public:
    static void Init(Napi::Env env, Napi::Object& exports);

    Subscriber(const Napi::CallbackInfo& info);
    void addReceiveCallback(const Napi::CallbackInfo& info);
    void addEventCallback(const Napi::CallbackInfo& info);

  private:
    void onReceive(const char* topicName, const struct eCAL::SReceiveCallbackData* data);
    void onEvent(const char* topicName, const struct eCAL::SSubEventCallbackData* data);

    eCAL::CSubscriber _sub;
    Napi::ThreadSafeFunction _tsFuncReceiveCb;
    Napi::ThreadSafeFunction _tsFuncEventCb;
};
