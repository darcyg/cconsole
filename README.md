# CConsole #

## Overview ##
CConsole is a monitoring application for viewing resource usage of processes (already running or to be run). It is composed of a GUI (cview) and a monitoring (backend) process (cmonitor).

## Build ##
### Dependencies ###
* [Boost](http://www.boost.org/) 1.59
* [googletest](https://github.com/google/googletest) version at 2016.03.30
* [CMake](https://cmake.org/) 2.8.12 or newer

### Requirements ###
The build requires the following envvars to be set in the building environment
* ***BOOST_HOME***: full path to the directory holding include and lib for Boost library (1.59+)
* ***GOOGLE_TEST_HOME***: full path to the built googletest library

### Build procedure ###
Under /path/to/sources issue the following command:

```bash
./cmk
```

Which will build all targets and place them under /path/to/sources/../build


## Process information ##
CMonitor publishes data (periodically) corresponding to a process identified by its ***PID***. The following data is to be sampled:
* ***pid***
* ***start_time***
* ***state***
* ***usage*** [%]
* ***utime*** [jiffies]
* ***stime*** [jiffies]
* ***num_thread***
* ***virtual_memory***  [kB]
* ***resident_memory*** [kB]

This data is obtained from the following sources:
* ***/proc/stat***: cpu total 
* ***/proc/$pid/status***: memory specific data
* ***/proc/$pid/stat***: other

## Contact ##
* iszekeres.x@gmail.com

