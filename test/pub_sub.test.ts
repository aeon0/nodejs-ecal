// TODO: This currently does not work properly.
//       For some reason we cant create a working publisher
//       and subscriber in the same test... why?
import * as ecal from ".."
import * as promises from "timers/promises"
import * as util from "node:util"
import { describe, it } from "mocha"
import { expect } from "chai"


var counter = 0;

async function createSub() {
  // Create subscriber
  var sub = new ecal.Subscriber("ecal-nodejs-test");
  const receiveCb: ecal.ReceiveCallback = (topic, msg) => {
    const strMsg = Buffer.from(msg).toString("utf8");
    console.log("Received from " + topic + ": " + strMsg);
  };
  sub.addReceiveCallback(receiveCb);
}

async function createPub() {
    // Create publisher
    var pub = new ecal.Publisher("ecal-nodejs-test");
    counter = 0;
    for (var i = 0; i < 10; i++) {
      const msg = "Hello World " + i;
      console.log("Send: " + msg);
      const data = new util.TextEncoder().encode(msg);
      pub.send(data.buffer, i);
      await promises.setTimeout(300);
    }
}

describe("Test Publisher and Subscriber", () => {
  it("publisher_subscriber", async () => {
    ecal.initialize();

    createSub();
    await createPub();

    // expect(counter).to.be.greaterThan(0);
    ecal.finalize();
  });
});
