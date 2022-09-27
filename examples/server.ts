import * as ecal from "../index"
import * as promises from "timers/promises"


const main = async () => {
  ecal.initialize();
  var server = new ecal.Server("nodejs-example-service");

  const methodCb: ecal.MethodCallback = (method, reqType, respType, request) => {
    console.log("");
    console.log("[Server] Method: " + method);
    console.log("reqType: " + reqType);
    console.log("respType: " + respType);
    console.log("request: " + request);
    const response = "Response from Server";
    return response;
  };
  server.addMethodCallback("echo", "", "", methodCb);

  const eventCb: ecal.ServerEventCallback = (name, event) => {
    console.log("[Server] Event from " + name);
    console.log(event);
  };
  server.addEventCallback(ecal.eCAL_Server_Event.server_event_connected, eventCb);
  server.addEventCallback(ecal.eCAL_Server_Event.server_event_disconnected, eventCb);

  while (ecal.ok()) {
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
