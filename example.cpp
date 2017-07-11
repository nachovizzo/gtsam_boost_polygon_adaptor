/* ---------------------------------------------------------------------------------
 * example.cpp

 *  Created on: Jul 10, 2017
 *      Author: ivizzo
 * --------------------------------------------------------------------------------- */

/**
* @brief
* This dummy module is to show how one should use the gtsam_boost_polygon adaptor
* with the gtsam based polygons.
* From the header file this are the data types :
*  -> typedef gtsam::Pose2 gtsam_point_t;
*  -> typedef std::vector<gtsam_point_t> gtsam_polygon_t;
*  -> typedef std::vector<gtsam_polygon_t> gtsam_polygon_set_t;
*
*/

/*|-----------------------------------------------------------------------------------|
  |  [1] - Include the adapter module                                                 |
  |-----------------------------------------------------------------------------------|*/
#include "gtsam_boost_polygon_adaptor.hpp"

/*|-----------------------------------------------------------------------------------|
  |  [2] - Write a simple test function                                               |
  |-----------------------------------------------------------------------------------|*/
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

/*|-----------------------------------------------------------------------------------|
  |  [3] - Write main function                                                        |
  |-----------------------------------------------------------------------------------|*/
int main(int argc, char **argv){

    // call your test function
    test();

    //say goodbye
    return 0;
}

/*|-----------------------------------------------------------------------------------|
  |  [4] - Build your test module                                                     |
  |-----------------------------------------------------------------------------------|*/

  /*
    The easiest way to build a simple
    module like this one, is invoke the
    c++ compiler from the command line.

    You need to provide extra information
    to the linker thouhg. You need to link
    the gtsam library and the boost system
    library as well(this is a dependency
    introduced by gtsam)

    $ g++ -std=c++11 example.cpp -lboost_system -lgtsam -o example

    $ ./example #runs the example

  */