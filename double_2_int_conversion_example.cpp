/* ---------------------------------------------------------------------------------
 * double_2_int_conversion_example.cpp

 *  Created on: Jul 11, 2017
 *      Author: ivizzo
 * --------------------------------------------------------------------------------- */

/**
* @brief
* This module is to show how you should deal with double coordinate types.
* The adaptor itself knows how to deal with doubles, but the methods from the
* boost::polygon library doesn't.
* See : http://www.boost.org/doc/libs/1_62_0/libs/polygon/doc/gtl_coordinate_concept.htm
* "In general a data type should define std::numeric_limits and be integer-like.
*  Floating point coordinate types are not supported by all the algorithms and generally
*  not suitable for use with the library at present."
*
* Note : for this simple example we will suppose double values with 2 decimal digits :
* double my_coordinate = 1.25f ;
*/
#include "gtsam_boost_polygon_adaptor.hpp"

// We will use this macro to convert from double to int
#define SCALE_FACTOR(X) 1e##X

// This crazy macro is only for making things easy to you,
// copy paste the output of the console, run it with MATLAB/GNU Octave
#define MATBLAB_MACRO       \
        "% plots \n"        \
        "x = a(:,1); \n"    \
        "y = a(:,2); \n"    \
        "figure() \n"       \
        "grid() \n"         \
        "plot(x,y,'b'); \n" \
        "hold() \n"         \
        " \n"               \
        "x = b(:,1); \n"    \
        "y = b(:,2); \n"    \
        " \n"               \
        "plot(x,y,'r'); \n" \
        "hold() \n"         \
        " \n"               \
        "x = c(:,1); \n"    \
        "y = c(:,2); \n"    \
        "hold() \n"         \
        "plot(x,y,'g'); \n" \

void test(void){
    // declare your polygon, with your custom data type
    gtsam_polygon_set_t my_polygon;

    // fill your polygon with some random object, like a rectangle
    my_polygon += boost::polygon::rectangle_data<double>(5.25, 5.25, 15.45, 15.45);

    // Scale your polygon, in order to let the library work with integer numerics
    my_polygon = gtl::scale_up(my_polygon, SCALE_FACTOR(2));

    // do some crazy magic thing, like bloat and shrink the polygon
    gtsam_polygon_set_t my_deflated_polygon, my_inflated_polygon;
    int ammount_of_offsetting = 1;

    // Since we scale upm our original polygon now we need to do the same with
    // the ammount of offseting we want to apply.
    ammount_of_offsetting = ammount_of_offsetting * SCALE_FACTOR(2);

    // Now we can have with out polygons as always
    my_deflated_polygon += (my_polygon - ammount_of_offsetting); //deflates the polygon by "1"
    my_inflated_polygon += (my_polygon + ammount_of_offsetting); //inflates the polygon by "1"

    // output your results to the std output.
    std::cout << "a" << my_polygon          << std::endl
              << "b" << my_deflated_polygon << std::endl
              << "c" << my_inflated_polygon << std::endl;

    std::cout << MATBLAB_MACRO << std::endl ;

    // now that you have your deflated/inflated polygons you should back to doubles
    my_polygon          = gtl::scale_down(my_polygon, 100) ;
    my_deflated_polygon = gtl::scale_down(my_deflated_polygon, 100) ;
    my_inflated_polygon = gtl::scale_down(my_inflated_polygon, 100) ;

    // output your results to the std output.
    std::cout << "a" << my_polygon          << std::endl
              << "b" << my_deflated_polygon << std::endl
              << "c" << my_inflated_polygon << std::endl;

    std::cout << MATBLAB_MACRO << std::endl ;
}

int main(int argc, char **argv){

    // call your test function
    test();

    //say goodbye
    return 0;
}