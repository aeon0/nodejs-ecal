const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var client = new ecal.Client("nodejs-example-service");

  const methodName = "echo";
  const request = "Hello from Client";

  const responseCb = (res) => {
    console.log("[Client] Response:");
    console.log(res);
  };
  client.addResponseCallback(responseCb);

  const eventCb = (name, event) => {
    console.log("");
    console.log("[Client] Event from " + name);
    console.log(event);
  };
  client.addEventCallback(0, eventCb); // Connected event
  client.addEventCallback(1, eventCb); // Disconnected event

  while (ecal.ok()) {
    console.log("call async: " + methodName +  ", request: " + request);
    client.callAsync(methodName, request, -1);
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
