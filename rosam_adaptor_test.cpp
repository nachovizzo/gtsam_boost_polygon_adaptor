/*
 * rosam_adaptor_test.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: ivizzo
 */

#include "rosam_boost_adaptor.hpp"
#include <iostream>

//first lets turn our polygon usage code into a generic
//function parameterized by polygon type
template<typename Polygon>
  void
  test_polygon ()
  {
    //lets construct a 10x10 rectangle shaped polygon
    typedef typename gtl::polygon_traits<Polygon>::point_type Point;
    gtl::rectangle_data<double> rect;


    Point pts[] =
	  { gtl::construct<Point> (0.0, 0.0), gtl::construct<Point> (10.0, 0.0),
	      gtl::construct<Point> (10.0, 10.0), gtl::construct<Point> (0.0,
									 10.0) };
    Polygon poly;
    gtl::set_points (poly, pts, pts + 4);

    std::cout << "Poly = [" << std::endl;

    for (int i = 0; i < poly.size(); ++i) {
	std::cout << poly[i].x() << "," <<poly[i].y() << ";" <<std::endl;
    }
    std::cout << "];" <<std::endl;


    //now lets see what we can do with this polygon
    assert(gtl::area (poly) == 100.0f);
    assert(gtl::contains (poly, gtl::construct<Point> (5, 5)));
    assert(!gtl::contains (poly, gtl::construct<Point> (15, 5)));
    assert(gtl::extents (rect, poly)); //get bounding box of poly
    assert(gtl::equivalence (rect, poly)); //hey, that's slick
    assert(gtl::winding (poly) == gtl::COUNTERCLOCKWISE);
    assert(gtl::perimeter (poly) == 40.0f);

    //add 5 to all coords of poly
    gtl::convolve (poly, gtl::construct<Point> (5.0, 5.0));

    std::cout << "Poly after convolve= [" << std::endl;

    for (int i = 0; i < poly.size(); ++i) {
	std::cout << poly[i].x() << "," <<poly[i].y() << ";" <<std::endl;
    }
    std::cout << "];" <<std::endl;

    gtl::set_points (rect, gtl::point_data<double> (5.0, 5.0),
		     gtl::point_data<double> (15.0, 15.0));
    assert(gtl::equivalence (poly, rect));


    //multiply all coords of poly by 2
    gtl::scale_up (poly, 2.0);
    gtl::set_points (rect, gtl::point_data<double> (10.0, 10.0),
		     gtl::point_data<double> (30.0, 30.0));
    assert(gtl::equivalence (poly, rect));
  }

//once again we make our usage of the library generic
//and parameterize it on the polygon set type
template <typename PolygonSet>
void test_polygon_set() {
  using namespace gtl;
#if 1
  PolygonSet ps;
  ps += rectangle_data<double>(0.0, 0.0, 10.0, 10.0);
  assert(area(ps) == 100.0f);

  PolygonSet ps2;
  ps2 += rectangle_data<double>(5.0, 5.0, 15.0, 15.0);
  PolygonSet ps3;
  assign(ps3, ps * ps2);
  PolygonSet ps4;
  ps4 += ps + ps2;

  // Let's print this polygon to se what it has
  // ps += 5;
  std::cout << "ps  = " << std::endl << ps  << std::endl;
  std::cout << "ps2 = " << std::endl << ps2 << std::endl;
  std::cout << "ps3 = " << std::endl << ps3 << std::endl;
  std::cout << "ps4 = " << std::endl << ps4 << std::endl;
  assert(area(ps4) == area(ps) + area(ps2) - area(ps3));
  assert(equivalence((ps + ps2) - (ps * ps2), ps ^ ps2));
  rectangle_data<double> rect;
  assert(extents(rect, ps ^ ps2));
  assert(area(rect) == 225);
  assert(area(rect ^ (ps ^ ps2)) == area(rect) - area(ps ^ ps2));
#endif
}

template <typename PolygonSet>
void test_polygon_set(PolygonSet &ps)
{
  ps += gtl::rectangle_data<double>(0.0, 0.0, 10.0, 10.0);
  std::cout << "ps  = " << std::endl
            << ps << std::endl;

  std::cout << "ps area = " << gtl::area(ps) << std::endl;
}

void test_polygon_set(rosam_polygon_set_t &ps)
{
  typedef typename gtl::polygon_traits<rosam_polygon_t>::point_type Point;

  Point pts[] =
      {gtl::construct<Point>(0.0, 0.0), gtl::construct<Point>(10.0, 0.0),
       gtl::construct<Point>(10.0, 10.0), gtl::construct<Point>(0.0,
                                                                10.0)};

  rosam_polygon_t poly;

  gtl::set_points(poly, pts, pts + 4);

  ps += poly;

  std::cout << "->ps  = " << std::endl
            << ps << std::endl;

  std::cout << "ps area = " << gtl::area(ps) << std::endl;
}

int
main (int argc, char **argv)
{
  std::cout << "Launching Rosam Adaptor test..." << std::endl<< std::endl;

  //First we test our mapped concept of polygon (a.k.a rosam_polygon_t)
#if 0
  test_polygon<rosam_polygon_t> ();
  std::cout << "[OK] - User defined polygon tested." << std::endl;

  test_polygon_set<gtl::polygon_set_data<double> > ();
  std::cout << "[OK] - Boost::polygon poygon_set_data tested." << std::endl;

  test_polygon_set<rosam_polygon_set_t> ();
  std::cout << "[OK] - User definded poygon_set_data tested." << std::endl;
#endif

  rosam_polygon_set_t rosam_polygon;
  test_polygon_set(rosam_polygon);
  std::cout << "[OK] - User definded poygon_set_data tested." << std::endl;

//  test_polygon_set<rosam_polygon_set_t> ();

}

