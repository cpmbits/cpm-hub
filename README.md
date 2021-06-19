# CPM Hub ![C/C++ CI](https://github.com/jorsanpe/cpm-hub/workflows/C/C++%20CI/badge.svg?branch=master)

The cpm-hub service is the reference backend used by cpm to deliver dependencies. 

### Compilation
cpm Hub depends on cpm for building its sources. Install cpm using pip:

`pip install --upgrade cpm-cli`

Then you can simply build the project:

`cpm build`

The project binary will be found in the build folder as `build/cpm-hub`. 
