const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var server = new ecal.Server("nodejs-example-service");

  const methodCb = (method, reqType, respType, request) => {
    console.log("");
    console.log("[Server] Method: " + method);
    console.log("reqType: " + reqType);
    console.log("respType: " + respType);
    console.log("request: " + request);
    const response = "Response from Server";
    return response;
  };
  server.addMethodCallback("echo", "", "", methodCb);

  const eventCb = (name, event) => {
    console.log("[Server] Event from " + name);
    console.log(event);
  };
  server.addEventCallback(0, eventCb); // Connected event
  server.addEventCallback(1, eventCb); // Disconnected event

  while (ecal.ok()) {
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
