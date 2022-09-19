const addon = require('bindings')('nodejsEcal');

// Expose API
exports.Subscriber = addon.Subscriber;
exports.Publisher = addon.Publisher;
exports.initialize = addon.initialize;
exports.ok = addon.ok;
exports.finalize = addon.finalize;
