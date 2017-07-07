## rosam_boost_polygon_adaptor

This is a single module to map a polygon based on gtsam::Pose2 concept into the boost.polygon library.

## Instructions to build module (Tested on a Ubuntu 16.04 machine)

As we will be using gtsam, we will need first to install it on our system.
You can read the gtsam installation file, is quite useful. And we will have everything
to build our stuff if we have everything to build gtsam

## Build gtsam

$ wget https://research.cc.gatech.edu/borg/sites/edu.borg/files/downloads/gtsam-3.2.1.tgz

$ tar -xvf gtsam-3.2.1.tgz 

$ cd gtsam-3.2.1/

$ mkdir build

$ cd build

$ cmake ..

Now you need to patch this file(if you have an old version of boost, following this instructions : https://svn.boost.org/trac10/ticket/12126)

$ vi gtsam-3.2.1/gtsam/base/FastSet.h and add #include<boost/serialization/serialization.hpp>

$ make check #(optional, runs unit tests)

$ sudo make install

## Build the adaptor and run the unit test

$ git clone https://github.com/IgnacioVizz0/rosam_boost_polygon_adaptor.git

$ cd rosam_boost_polygon_adaptor/build

$ make all

$ ./boost_polygon_traits # this run the unit test
