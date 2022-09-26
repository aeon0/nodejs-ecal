const assert = require("assert");
const ecal = require(".."); // loads ../index.js

const sleep = ms => new Promise(resolve => setTimeout(resolve, ms));

describe("Test Subscriber", () => {
  it("subscriber", async () => {
    ecal.initialize();

    var sub = new ecal.Subscriber("ecal-nodejs-test");
    var counter = 0;
    sub.addReceiveCallback((topic, msg) => {
      const strMsg = Buffer.from(msg).toString("utf8");
      console.log("Recived: " + strMsg);
      counter++;
    });

    await sleep(1500);
    // We should recive at least one message
    assert(counter > 0);
    ecal.finalize();
  });
});
