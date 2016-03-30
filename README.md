# cconsole

## Overview
CConsole is a monitoring application for viewing resource usage of processes (already running or to be run). It is composed of a GUI (cview) and a monitoring (backend) process (cmonitor).

## Build
### Dependencies
* [Boost](http://www.boost.org/) 1.59
* [googletest](https://github.com/google/googletest) version at 2016.03.30

### Requirements
The build requires the following envvars to be set in the building environment
* BOOST_HOME: full path to the directory holding include and lib for Boost library (1.59+)
* GOOGLE_TEST_HOME: full path to the built googletest library

### Build procedure
Under /path/to/sources issue the following command:

```bash
./cmk
```

Which will build all targets and place them under /path/to/sources/../build


## Contact
* iszekeres.x@gmail.com
