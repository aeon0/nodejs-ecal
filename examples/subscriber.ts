import * as ecal from "../index"
import * as promises from "timers/promises"


const main = async () => {
  ecal.initialize();
  var sub = new ecal.Subscriber("nodejs-ecal-example");

  const receiveCb: ecal.ReceiveCallback = (topic, data) => {
    const strMsg = Buffer.from(data).toString("utf8");
    console.log("Received from " + topic + ": " + strMsg);
  };
  sub.addReceiveCallback(receiveCb);

  const eventCb: ecal.SubEventCallback = (topic, event) => {
    console.log("Event from " + topic);
    console.log(event);
  }
  sub.addEventCallback(ecal.eCAL_Subscriber_Event.sub_event_connected, eventCb);
  sub.addEventCallback(ecal.eCAL_Subscriber_Event.sub_event_disconnected, eventCb);

  while (ecal.ok()) {
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
