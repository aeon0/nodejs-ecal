import * as ecal from "../index"
import * as promises from "timers/promises"


const main = async () => {
  ecal.initialize();
  var client = new ecal.Client("nodejs-example-service");

  const methodName: string = "echo";
  const request: string = "Hello from Client";

  const responseCb: ecal.ResponseCallback = (res) => {
    console.log("[Client] Response:");
    console.log(res);
  };
  client.addResponseCallback(responseCb);

  const eventCb: ecal.ClientEventCallback = (name, event) => {
    console.log("");
    console.log("[Client] Event from " + name);
    console.log(event);
  };
  client.addEventCallback(ecal.eCAL_Client_Event.client_event_connected, eventCb);
  client.addEventCallback(ecal.eCAL_Client_Event.client_event_disconnected, eventCb);

  while (ecal.ok()) {
    console.log("call async: " + methodName +  ", request: " + request);
    client.callAsync(methodName, request, -1);
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
