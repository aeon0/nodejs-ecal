const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var sub = new ecal.Subscriber("nodejs-ecal-example");

  const receiveCb = (topic, msg) => {
    const strMsg = Buffer.from(msg).toString("utf8");
    console.log("Recived from " + topic + ": " + strMsg);
  };
  sub.addReceiveCallback(receiveCb);

  const eventCb = (topic, event) => {
    console.log("Event from " + topic);
    console.log(event);
  };
  sub.addEventCallback(0, eventCb); // Connected event
  sub.addEventCallback(1, eventCb); // Disconnected event

  while (ecal.ok()) {
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
