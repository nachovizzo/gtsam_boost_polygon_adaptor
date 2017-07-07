/*
 * rosam_adaptor_test.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: ivizzo
 */

#include "rosam_boost_adaptor.hpp"
#include <cassert>
#include <iostream>

//lets make the body of main from point_usage.cpp
//a generic function parameterized by point type
template <typename Point, typename coordinate_type>
void test_point()
{
  //constructing a gtl point
  coordinate_type x = 10;
  coordinate_type y = 20;
  //Point pt(x, y);
  Point pt = gtl::construct<Point>(x, y);
  assert(gtl::x(pt) == 10);
  assert(gtl::y(pt) == 20);

  //a quick primer in isotropic point access
  typedef gtl::orientation_2d O;
  using gtl::HORIZONTAL;
  using gtl::VERTICAL;
  O o = HORIZONTAL;
  assert(gtl::x(pt) == gtl::get(pt, o));

  o = o.get_perpendicular();
  assert(o == VERTICAL);
  assert(gtl::y(pt) == gtl::get(pt, o));

  gtl::set(pt, o, 30);
  assert(gtl::y(pt) == 30);

  //using some of the library functions
  //Point pt2(10, 30);
  Point pt2 = gtl::construct<Point>(10, 30);
  assert(gtl::equivalence(pt, pt2));

  gtl::transformation<coordinate_type> tr(gtl::axis_transformation::SWAP_XY);
  gtl::transform(pt, tr);
  assert(gtl::equivalence(pt, gtl::construct<Point>(30, 10)));

  gtl::transformation<coordinate_type> tr2 = tr.inverse();
  assert(tr == tr2); //SWAP_XY is its own inverse transform

  gtl::transform(pt, tr2);
  assert(gtl::equivalence(pt, pt2)); //the two points are equal again

  gtl::move(pt, o, 10); //move pt 10 units in y
  assert(gtl::euclidean_distance(pt, pt2) == 10.0f);

  gtl::move(pt, o.get_perpendicular(), 10); //move pt 10 units in x
  assert(gtl::manhattan_distance(pt, pt2) == 20);
}

//first lets turn our polygon usage code into a generic
//function parameterized by polygon type
template <typename Polygon, typename coordinate_type>
void test_polygon()
{
  //lets construct a 10x10 rectangle shaped polygon
  typedef typename gtl::polygon_traits<Polygon>::point_type Point;
  gtl::rectangle_data<coordinate_type> rect;

  Point pts[] =
      {gtl::construct<Point>(0, 0), gtl::construct<Point>(10, 0),
       gtl::construct<Point>(10, 10), gtl::construct<Point>(0, 10)};
  Polygon poly;
  gtl::set_points(poly, pts, pts + 4);

  //now lets see what we can do with this polygon
  assert(gtl::area(poly) == 100.0f);
  assert(gtl::contains(poly, gtl::construct<Point>(5, 5)));
  assert(!gtl::contains(poly, gtl::construct<Point>(15, 5)));
  assert(gtl::extents(rect, poly));     //get bounding box of poly
  assert(gtl::equivalence(rect, poly)); //hey, that's slick
  assert(gtl::winding(poly) == gtl::COUNTERCLOCKWISE);
  assert(gtl::perimeter(poly) == 40.0f);

  //add 5 to all coords of poly
  gtl::convolve(poly, gtl::construct<Point>(5.0, 5.0));
  gtl::set_points(rect, gtl::point_data<coordinate_type>(5.0, 5.0),
                  gtl::point_data<coordinate_type>(15.0, 15.0));
  assert(gtl::equivalence(poly, rect));

  //multiply all coords of poly by 2
  gtl::scale_up(poly, 2.0);
  gtl::set_points(rect, gtl::point_data<coordinate_type>(10.0, 10.0),
                  gtl::point_data<coordinate_type>(30.0, 30.0));
  assert(gtl::equivalence(poly, rect));
}

//once again we make our usage of the library generic
//and parameterize it on the polygon set type
template <typename PolygonSet, typename coordinate_type>
void test_polygon_set()
{
  using namespace gtl;
  PolygonSet ps;
  ps += rectangle_data<coordinate_type>(0, 0, 10, 10);

  PolygonSet ps2;
  ps2 += rectangle_data<coordinate_type>(5, 5, 15, 15);
  PolygonSet ps3;
  assign(ps3, ps * ps2);
  PolygonSet ps4;
  ps4 += ps + ps2;

  assert(area(ps) == 100);
  assert(area(ps4) == area(ps) + area(ps2) - area(ps3));
  assert(equivalence((ps + ps2) - (ps * ps2), ps ^ ps2));
  rectangle_data<coordinate_type> rect;
  assert(extents(rect, ps ^ ps2));
  assert(area(rect) == 225);
  assert(area(rect ^ (ps ^ ps2)) == area(rect) - area(ps ^ ps2));
}

int main(int argc, char **argv)
{
  std::cout << "Launching Rosam Adaptor test..." << std::endl<< std::endl;

  //First test the Point concept
  test_point<gtl::point_data<double>,double >();
  std::cout << "[OK] - boost::polygon::point_data tested." << std::endl;

  test_point<rosam_point_t,double >();
  std::cout << "[OK] - User definded point_data tested." << std::endl;

  //Then we test our mapped concept of polygon (a.k.a rosam_polygon_t)
  test_polygon<gtl::polygon_data<double>,double > ();
  std::cout << "[OK] - boost::polygon::polygon_data tested." << std::endl;

  test_polygon<rosam_polygon_t,double> ();
  std::cout << "[OK] - User defined polygon tested." << std::endl;

  //Finnaly we test the polygon_set_concept
  test_polygon_set<gtl::polygon_set_data<double>,double > ();
  std::cout << "[OK] - Boost::polygon poygon_set_data tested." << std::endl;

  test_polygon_set<rosam_polygon_set_t, double> ();
  std::cout << "[OK] - User definded poygon_set_data tested." << std::endl;

}

