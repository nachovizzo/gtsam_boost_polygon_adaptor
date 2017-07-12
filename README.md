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
#include <boost/serialization/serialization.hpp> //<----------------------
#include <boost/serialization/set.hpp>
```
```sh
$ make check #(optional, runs unit tests)
$ sudo make install
```

## Usage
There is a simple example(example.cpp) in the root directory in order
to learn how to use the adaptor. Basically it consist in 4 simple steps :
 1. Include the adapter module
    ```cpp
    #include "gtsam_boost_polygon_adaptor.hpp"
    ```
 2. Write a simple test function
    ```cpp
    void test(void){
        // declare your polygon, with your custom data type
        gtsam_polygon_set_t my_polygon;

        // fill your polygon with some random object, like a rectangle
        my_polygon += boost::polygon::rectangle_data<double>(0, 0, 10, 10);

        // do some crazy magic thing, like bloat and shrink the polygon
        gtsam_polygon_set_t my_deflated_polygon, my_inflated_polygon;
        my_deflated_polygon += (my_polygon - 1); //deflates the polygon by 1
        my_inflated_polygon += (my_polygon + 1); //inflates the polygon by 1

        // output your results to the std output.
        std::cout << "My polygon is = " << my_polygon << std::endl
                << "My deflated polygon is = " << my_deflated_polygon << std::endl
                << "My inflated polygon is = " << my_inflated_polygon << std::endl;

        // have fun
        std::cout << "Now use this module to do your job!"  << std::endl ;
    }
    ```
 3. Write main function to test it
    ```cpp
    int main(int argc, char **argv){

        // call your test function
        test();

        //say goodbye
        return 0;
    }
    ```
 4. Build and run your test module:
    ```sh
    $ g++ -std=c++11 example.cpp -lboost_system -lgtsam -o example
    $ ./example #runs the example
    ```

## Double to int conversions
In order to use the *boost::polygon* methods, we need to use integer type coordinates(see :
[Coordinate Concept](http://www.boost.org/doc/libs/1_62_0/libs/polygon/doc/gtl_coordinate_concept.htm)):

> "In general a data type should define std::numeric_limits and be integer-like.
> **Floating point coordinate types are not supported by all the algorithms** and generally
> not suitable for use with the library at present."

So, I wrote a simple example in order to help you with how you should work with doubles and integers.

 1. Install xclip(If you don't have it yet) will be useful in the future as well
 ```sh
 $ sudo apt install xlcip
 ```
2. Do everything in one step! (build&run&copy)
```sh
$ g++ -std=c++11 double_2_int_conversion_example.cpp -lboost_system -lgtsam -o example && \
  ./example |  xclip -selection clipboard #FANCY!
```
3. Open GNU/Octave for example, paste the results you already have in your clipboard and plot!

    Integers:

    ![Integer](https://preview.ibb.co/joxYNv/double.png)

    Doubles:

    ![Double](https://preview.ibb.co/dn0g9a/double.png)

## Build the adapter unit test
```sh
$ git clone
$ cd gtsam_boost_polygon_adaptor/build
$ make all
$ ./gtsam_boost_polygon_adaptor_test # this runs the unit test
```