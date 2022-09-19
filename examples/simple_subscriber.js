const ecal = require(".."); // if installed with npm its require("nodejs-ecal")
const promises = require("timers/promises");

const main = async () => {
  ecal.initialize();
  var sub = new ecal.Subscriber("ecal_nodejs_test");

  sub.addReceiveCallback((msg) => {
    const strMsg = Buffer.from(msg).toString("utf8");
    console.log("Recived: " + strMsg);
  });

  while (ecal.ok()) {
    await promises.setTimeout(1000);
  }
  ecal.finalize();
}
main();
