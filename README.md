## gtsam_boost_polygon_adaptor

This is a single module to map a polygon based on gtsam::Pose2 concept into the boost.polygon library.
## Dependencies

- [Gtsam](https://research.cc.gatech.edu/borg/sites/edu.borg/files/downloads/gtsam-3.2.1.tgz) = 3.2.1
- [Boost](http://www.boost.org/users/download/) >= 1.43 (Ubuntu: `sudo apt-get install libboost-all-dev`)
- [CMake](http://www.cmake.org/cmake/resources/software.html) >= 2.6 (Ubuntu: `sudo apt-get install cmake`)

## Instructions to build module (Tested on a Ubuntu 16.04 machine)
As we will be using gtsam, we will need first to install it on our system.
You can read the gtsam installation file, is quite useful. And we will have everything
to build our stuff if we have everything to build gtsam
## Build gtsam
```sh
$ wget https://research.cc.gatech.edu/borg/sites/edu.borg/files/downloads/gtsam-3.2.1.tgz
$ tar -xvf gtsam-3.2.1.tgz
$ cd gtsam-3.2.1/
$ mkdir build
$ cd build
$ cmake ..
```
Now you need to patch this file(if you have an old version of boost, following this instructions : https://svn.boost.org/trac10/ticket/12126)
```sh
$ vi gtsam-3.2.1/gtsam/base/FastSet.h and add #include<boost/serialization/serialization.hpp>
```
```c++
/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    FastSet.h
 * @brief   A thin wrapper around std::set that uses boost's fast_pool_allocator.
 * @author  Richard Roberts
 * @date    Oct 17, 2010
 */

#pragma once

#include <gtsam/base/FastDefaultAllocator.h>

#include <set>
#include <iostream>
#include <string>
#include <cmath>
#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp> <----------------------
#include <boost/serialization/set.hpp>
```
```sh
$ make check #(optional, runs unit tests)
$ sudo make install
```

## Build the adaptor and run the unit test
```sh
$ git clone ssh://git@git.wardrobe.irobot.com:7999/~ivizzo/gtsam_boost_polygon_adaptor.git
$ cd gtsam_boost_polygon_adaptor/build
$ make all
$ ./boost_polygon_traits # this run the unit test
```
