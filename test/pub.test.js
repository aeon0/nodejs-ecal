const assert = require("assert");
const util = require('node:util');
const nodejsEcal = require(".."); // loads ../index.js

const sleep = ms => new Promise(resolve => setTimeout(resolve, ms));

describe("Test Publisher", () => {
  it("publisher", async () => {
    nodejsEcal.initialize();
    var pub = new nodejsEcal.Publisher("ecal_nodejs_test");
    counter = 0;
    for (var i = 0; i < 10; i++) {
      msg = "Hello World " + i;
      console.log("Send: " + msg);
      const data = new util.TextEncoder("utf8").encode(msg);
      pub.send(data.buffer, i);
      await sleep(200);
    }
  });
});
