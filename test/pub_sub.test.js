const assert = require("assert");
const util = require('node:util');
const promises = require("timers/promises");
const ecal = require(".."); // loads ../index.js

// TODO: for some reason we cant create a working publisher and subscriber in the same test...
//       why?
async function createSub() {
  // Create subscriber
  var sub = new ecal.Subscriber("ecal-nodejs-test");
  const receiveCb = (topic, msg) => {
    const strMsg = Buffer.from(msg).toString("utf8");
    console.log("Recived from " + topic + ": " + strMsg);
  };
  sub.addReceiveCallback(receiveCb);
}

async function createPub() {
    // Create publisher
    var pub = new ecal.Publisher("ecal-nodejs-test");
    counter = 0;
    for (var i = 0; i < 10; i++) {
      msg = "Hello World " + i;
      console.log("Send: " + msg);
      const data = new util.TextEncoder("utf8").encode(msg);
      pub.send(data.buffer, i);
      await promises.setTimeout(300);
    }
}

describe("Test Publisher and Subscriber", () => {
  it("publisher_subscriber", async () => {
    ecal.initialize();

    createSub();
    await createPub();

    // assert(counter > 0);
    ecal.finalize();
  });
});
