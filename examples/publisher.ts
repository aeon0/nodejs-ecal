import * as ecal from "../index"
import * as promises from "timers/promises"
import * as util from "node:util"


const main = async () => {
  ecal.initialize();
  var pub = new ecal.Publisher("nodejs-ecal-example");

  const eventCb: ecal.PubEventCallback = (topic, event) => {
    console.log("Event from " + topic);
    console.log(event);
  };
  pub.addEventCallback(ecal.eCAL_Publisher_Event.pub_event_connected, eventCb);
  pub.addEventCallback(ecal.eCAL_Publisher_Event.pub_event_disconnected, eventCb);

  var counter = 0;
  while (ecal.ok()) {
    const msg = "Hello World " + counter;
    console.log("Sending: " + msg);
    const data = new util.TextEncoder().encode(msg);
    pub.send(data.buffer, -1);
    await promises.setTimeout(1000);
    counter++;
  }
  ecal.finalize();
}
main();
