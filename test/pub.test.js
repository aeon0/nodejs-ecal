const assert = require("assert");
const util = require('node:util');
const ecal = require(".."); // loads ../index.js

const sleep = ms => new Promise(resolve => setTimeout(resolve, ms));

describe("Test Publisher", () => {
  it("publisher", async () => {
    ecal.initialize();
    var pub = new ecal.Publisher("ecal-nodejs-test");
    counter = 0;
    for (var i = 0; i < 10; i++) {
      msg = "Hello World " + i;
      console.log("Send: " + msg);
      const data = new util.TextEncoder("utf8").encode(msg);
      pub.send(data.buffer, i);
      await sleep(300);
    }
    ecal.finalize();
  });
});
