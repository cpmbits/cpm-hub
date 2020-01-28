# CPM Hub Plugin Repository
CPM is a generic manager for C and C++ projects. The CPM plugin repository contains the package index 
used by CPM. 

## Custom CPM Hub installations
You can download and create your own installation of the CPM Hub Plugin Repository for keeping your
proprietary plugins.

### Compilation
CPM Hub depends on CPM for building its sources. Install CPM using pip:

`pip install cpm`

Then you can simply build the project:

`cpm build`

The project binary will be found in the root folder as `cpm-hub`. 
