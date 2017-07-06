/*
 * main.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: ivizzo
 *
 *  This module is only intended for training purposes
 *
 */

#include <iostream>
#include <cassert>
#include <boost/polygon/polygon.hpp>
#include <boost/polygon/detail/minkowski.hpp>
#include <gtsam/geometry/Pose2.h>
namespace gtl = boost::polygon;

typedef std::vector<gtsam::Point2> rosam_polygon_t;

//first lets turn our polygon usage code into a generic
//function parameterized by polygon type
template<typename Polygon>
  void
  test_polygon ()
  {
    //lets construct a 10x10 rectangle shaped polygon
    typedef typename gtl::polygon_traits<Polygon>::point_type Point;
    Point pts[] =
      { gtl::construct<Point> (0, 0), gtl::construct<Point> (10, 0),
	  gtl::construct<Point> (10, 10), gtl::construct<Point> (0, 10) };
    Polygon poly;
    gtl::set_points (poly, pts, pts + 4);

    //now lets see what we can do with this polygon
    assert(gtl::area (poly) == 100.0f);
    assert(gtl::contains (poly, gtl::construct<Point> (5, 5)));
    assert(!gtl::contains (poly, gtl::construct<Point> (15, 5)));
    gtl::rectangle_data<int> rect;
    assert(gtl::extents (rect, poly));	 //get bounding box of poly
    assert(gtl::equivalence (rect, poly)); //hey, that's slick
    assert(gtl::winding (poly) == gtl::COUNTERCLOCKWISE);
    assert(gtl::perimeter (poly) == 40.0f);

    std::cout << gtl::area (poly) << std::endl;
    using gtl::detail::minkowski_offset;
    gtl::resize (poly, 1);

    std::cout << gtl::area (poly) << std::endl;

    //add 5 to all coords of poly
    gtl::convolve (poly, gtl::construct<Point> (5, 5));
    //multiply all coords of poly by 2
    gtl::scale_up (poly, 2);
    gtl::set_points (rect, gtl::point_data<int> (10, 10),
		     gtl::point_data<int> (30, 30));
    assert(gtl::equivalence (poly, rect));
  }

//we have to get gtsam::Point2 working with boost polygon to make our polygon
//that uses gtsam::Point2 working with boost polygon
namespace boost
{
  namespace polygon
  {
    template<>
      struct geometry_concept<gtsam::Point2>
      {
	typedef point_concept type;
      };

    template<>
      struct point_traits<gtsam::Point2>
      {
	typedef int coordinate_type;

	static inline coordinate_type
	get (const gtsam::Point2 &point, orientation_2d orient)
	{
	  if (orient == HORIZONTAL)
	    return point.x ();
	  return point.y ();
	}
      };

    template<>
      struct point_mutable_traits<gtsam::Point2>
      {
	typedef int coordinate_type;

	static inline void
	set (gtsam::Point2 &point, orientation_2d orient, int value)
	{
	  if (orient == HORIZONTAL)
	    point = gtsam::Point2 (value, 0);
	  else
	    point = gtsam::Point2 (0, value);
	}
	static inline gtsam::Point2
	construct (int x_value, int y_value)
	{
	  return gtsam::Point2 (x_value, y_value);
	}
      };
  }
}

//we need to specialize our polygon concept mapping in boost polygon
namespace boost
{
  namespace polygon
  {
//first register CPolygon as a polygon_concept type
    template<>
      struct geometry_concept<rosam_polygon_t>
      {
	typedef polygon_concept type;
      };

    template<>
      struct polygon_traits<rosam_polygon_t>
      {
	typedef int coordinate_type;
	typedef rosam_polygon_t::const_iterator iterator_type;
	typedef gtsam::Point2 point_type;

	// Get the begin iterator
	static inline iterator_type
	begin_points (const rosam_polygon_t &t)
	{
	  return t.begin ();
	}

	// Get the end iterator
	static inline iterator_type
	end_points (const rosam_polygon_t &t)
	{
	  return t.end ();
	}

	// Get the number of sides of the polygon
	static inline std::size_t
	size (const rosam_polygon_t &t)
	{
	  return t.size ();
	}

	// Get the winding direction of the polygon
	static inline winding_direction
	winding (const rosam_polygon_t &t)
	{
	  return unknown_winding;
	}
      };

    template<>
      struct polygon_mutable_traits<rosam_polygon_t>
      {
	//expects stl style iterators
	template<typename iT>
	  static inline rosam_polygon_t &
	  set_points (rosam_polygon_t &t, iT input_begin, iT input_end)
	  {
	    t.clear ();
	    t.insert (t.end (), input_begin, input_end);
	    return t;
	  }
      };
  }
}

int
main (int argc, char **argv)
{
  test_polygon<rosam_polygon_t> ();
  return 0;
}
