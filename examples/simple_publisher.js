const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const util = require("node:util");
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var pub = new ecal.Publisher("ecal_nodejs_test");
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
