/*
 * rosam_adaptor_test.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: ivizzo
 */

#include "rosam_boost_adaptor.hpp"
#include <iostream>
// /**
//  *  DECIME ALGO
//  */
// namespace boost
// {
//   namespace polygon
//   {
//     template<typename T>
//       std::ostream&
//       operator<< (std::ostream& o, const gtl::polygon_data<T>& poly)
//       {
// 	o << "Polygon = [ " << std::endl;
// 	for (typename polygon_data<T>::iterator_type itr = poly.begin ();
// 	    itr != poly.end (); ++itr)
// 	  {
// 	    if (itr != poly.begin ())
// 	      o << ";" << std::endl;
// 	    o << (*itr).get (HORIZONTAL) << "," << (*itr).get (VERTICAL);
// 	  }
// 	o << " ]; ";
// 	return o;
//       }
//     template<typename T>
//       std::ostream&
//       operator << (std::ostream& o, const polygon_set_data<T>& r)
//       {
// 	o << "Polygon Set Data { ";
// 	for (typename polygon_set_data<T>::iterator_type itr = r.begin ();
// 	    itr != r.end (); ++itr)
// 	  {
// 	    o << "<" << (*itr).first.first << ", " << (*itr).first.second
// 		<< ">:" << (*itr).second << " ";
// 	  }
// 	o << "} ";
// 	return o;
//       }
//   }
// }
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
  assert(area(ps4) == area(ps) + area(ps2) - area(ps3));
  assert(equivalence((ps + ps2) - (ps * ps2), ps ^ ps2));
  rectangle_data<double> rect;
  assert(extents(rect, ps ^ ps2));
  assert(area(rect) == 225);
  assert(area(rect ^ (ps ^ ps2)) == area(rect) - area(ps ^ ps2));
#endif
}

int
main (int argc, char **argv)
{
  std::cout << "Launching Rosam Adaptor test..." << std::endl<< std::endl;

  //First we test our mapped concept of polygon (a.k.a rosam_polygon_t)
//  test_polygon<gtl::polygon_set_data> ();
//  std::cout << "[OK] - User defined polygon tested." << std::endl;

  test_polygon<rosam_polygon_t> ();
  std::cout << "[OK] - User defined polygon tested." << std::endl;

  test_polygon_set<gtl::polygon_set_data<double> > ();
  std::cout << "[OK] - Boost::polygon poygon_set_data tested." << std::endl;

  //There is already a mapped concpet for a std::vector of(is_polygon_concept)
  test_polygon_set<rosam_polygon_set_t> ();
  std::cout << "[OK] - User definded poygon_set_data tested." << std::endl;

//  test_polygon_set<rosam_polygon_set_t> ();

}

