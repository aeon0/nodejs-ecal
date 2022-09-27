# Nodejs-eCal

eCAL wrapper for Nodejs. Check out ecal here: https://github.com/eclipse-ecal/ecal.

## Dependencies
CMake, some compiler, protobuf and eCAL musst be installed on the system. See docs: https://eclipse-ecal.github.io/ecal/getting_started/setup.html

```bash
# Linux
sudo add-apt-repository ppa:ecal/ecal-latest
sudo apt-get update
sudo apt-get install ecal cmake g++ libprotobuf-dev protobuf-compiler
```

## Usage
```bash
npm install -s nodejs-ecal
```
Note that only communication with ArrayBuffers is supported. This is because specific interface description (e.g. for protobuf) would need to be known at compile time as node addons ship as dynamic libs.

That means on Node.js side the user has to take care of data serialization.

See examples at: [simple_publisher.js](examples/simple_publisher.js), [simple_subscriber.js](examples/simple_subscriber.js)

## Dev
Project is using cmake-js: https://github.com/cmake-js/cmake-js.
```bash
# Install dependencies (nodejs must be installed)
npm install
# optional to use cmake-js command in terminal
npm install -g cmake-js

# Build
npm run install

# Test
npm run test

# Publish new versions
npm login
npm publish
```
