## rosam_boost_polygon_adaptor

This is a single module to map a polygon based on gtsam::Pose2 concept into the boost.polygon library.

## Instructions to build module (Tested on a Ubuntu 16.04 machine)

As we will be using gtsam, we will need first to install it on our system.
You can read the gtsam installation file, is quite useful. And we will have everything
to build our stuff if we have everything to build gtsam

## Build gtsam

$ git clone https://bitbucket.org/gtborg/gtsam.git

$ cd gtsam

$ mkdir build

$ cd build

$ cmake ..

$ make check #(optional, runs unit tests)

$ make install

## Build the adaptor and run the unit test

$ git clone https://github.com/IgnacioVizz0/rosam_boost_polygon_adaptor.git

$ cd rosam_boost_polygon_adaptor/build

$ make all

$ ./boost_polygon_traits # this run the unit test
