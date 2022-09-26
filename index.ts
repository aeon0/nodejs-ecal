const addon = require('bindings')('nodejsEcal');


// Class Subscriber
// =============================================================================
export type ReceiveCallback = (topic: string, data: ArrayBuffer) => void;
export enum eCAL_Subscriber_Event {
  sub_event_none = 0,
  sub_event_connected,
  sub_event_disconnected,
  sub_event_dropped,
  sub_event_timeout,
  sub_event_corrupted
}
export interface SSubEventCallbackData {
  type: eCAL_Subscriber_Event,
  time: number,
  clock: number
}
export type SubEventCallback = (topic: string, event: SSubEventCallbackData) => void;
export interface Subscriber {
  addReceiveCallback(callback: ReceiveCallback): void;
  addEventCallback(callback: SubEventCallback): void;
}

export var Subscriber: {
    new(topic: string): Subscriber
} = addon.Subscriber


// Expose API
// exports.Subscriber = addon.Subscriber;
// exports.Publisher = addon.Publisher;
// exports.initialize = addon.initialize;
// exports.ok = addon.ok;
// exports.finalize = addon.finalize;
