const addon = require('bindings')('nodejsEcal');


// Functions
// =============================================================================
export var ok: () => boolean = addon.ok;
export var finalize: () => void = addon.finalize;
export var initialize: () => void = addon.initialize;

// Class Subscriber
// =============================================================================
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
export type ReceiveCallback = (topic: string, data: ArrayBuffer) => void;
export interface Subscriber {
  addReceiveCallback(callback: ReceiveCallback): void;
  addEventCallback(callback: SubEventCallback): void;
}
export var Subscriber: {
    new(topic: string): Subscriber
} = addon.Subscriber


// Class Publisher
// =============================================================================
export enum eCAL_Publisher_Event {
  pub_event_none = 0,
  pub_event_connected,
  pub_event_disconnected,
  pub_event_dropped
}
export interface SPubEventCallbackData {
  type: eCAL_Publisher_Event,
  time: number,
  clock: number
}
export type PubEventCallback = (topic: string, event: SPubEventCallbackData) => void;
export interface Publisher {
  send(data: ArrayBuffer, timestamp: number): void;
  addEventCallback(callback: PubEventCallback): void;
}
export var Publisher: {
    new(topic: string): Publisher
} = addon.Publisher

// Class Server
// =============================================================================
export enum eCAL_Server_Event {
  server_event_none = 0,
  server_event_connected,
  server_event_disconnected 
}
export interface SServerEventCallbackData {
  type: eCAL_Server_Event,
  time: number,
}
export type ServerEventCallback = (serviceName: string, event: SServerEventCallbackData) => void;
export type MethodCallback = (method: string, reqType: string, respType: string, request: string) => string;
export interface Server {
  addMethodCallback(method: string, reqType: string, respType: string, callback: MethodCallback): void;
  addEventCallback(callback: ServerEventCallback): void;
}
export var Server: {
    new(serviceName: string): Server
} = addon.Server

// Class Client
// =============================================================================
export enum eCAL_Client_Event {
  client_event_none = 0,
  client_event_connected,
  client_event_disconnected,
  client_event_timeout
}
export interface SClientEventCallbackData {
  type: eCAL_Client_Event,
  time: number
}
export type ClientEventCallback = (serviceName: string, event: SClientEventCallbackData) => void;
export enum eCallState {
  call_state_none = 0,
  call_state_executed,
  call_state_failed
}
export interface SServiceResponse {
  hostName: string,
  serviceName: string,
  methodName: string,
  errorMsg: string,
  serviceId: string,
  retState: number,
  response: string,
  callState: eCallState,
}
export type ResponseCallback = (response: SServiceResponse) => void;
export interface Client {
  callAsync(method: string, request: string, timeout: number): void;
  addResponseCallback(callback: ResponseCallback): void;
  addEventCallback(callback: ClientEventCallback): void;
}
export var Client: {
    new(serviceName: string): Client
} = addon.Client
