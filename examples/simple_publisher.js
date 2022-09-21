const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const util = require("node:util");
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var pub = new ecal.Publisher("nodejs-ecal-example");

  const eventCb = (topic, event) => {
    console.log("Event from " + topic);
    console.log(event);
  };
  pub.addEventCallback(0, eventCb); // Connected event
  pub.addEventCallback(1, eventCb); // Disconnected event

  var counter = 0;
  while (ecal.ok()) {
    const msg = "Hello World " + counter;
    console.log("Sending: " + msg);
    const data = new util.TextEncoder("utf8").encode(msg);
    pub.send(data.buffer, -1);
    await promises.setTimeout(1000);
    counter++;
  }
  ecal.finalize();
}
main();
