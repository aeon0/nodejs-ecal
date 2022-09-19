const assert = require("assert");
const nodejsEcal = require(".."); // loads ../index.js

const sleep = ms => new Promise(resolve => setTimeout(resolve, ms));

describe("Test Subscriber", () => {
  it("subscriber", async () => {
    nodejsEcal.initialize();

    var sub = new nodejsEcal.Subscriber("ecal_nodejs_test");
    var counter = 0;
    sub.addReceiveCallback((msg) => {
      const strMsg = Buffer.from(msg).toString("utf8");
      console.log("Recived: " + strMsg);
      counter++;
    });

    await sleep(1200);
    // We should recive at least one message
    assert(counter > 0);
  });
});
