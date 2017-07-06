/*
 * main.cpp
 *
 *  Created on: Jun 30, 2017
 *      Author: ivizzo
 *
 *  This module is only intended for training purposes
 *
 */
#include <boost/polygon/polygon.hpp>
#include <iostream>
#include <cassert>
#include <gtsam/geometry/Pose2.h>
#include <list>
#include <time.h>
#include <deque>

namespace gtl = boost::polygon;
using namespace boost::polygon::operators;
typedef std::vector<gtsam::Point2> rosam_polygon_t;

namespace boost
{
  namespace polygon
  {

    template<typename T>
      std::ostream&
      operator<< (std::ostream& o, const gtl::polygon_data<T>& poly)
      {
        o << "Polygon = [ " << std::endl;
        for (typename polygon_data<T>::iterator_type itr = poly.begin ();
            itr != poly.end (); ++itr)
          {
            if (itr != poly.begin ())
              o << ";" << std::endl;
            o << (*itr).get (HORIZONTAL) << "," << (*itr).get (VERTICAL);
          }
        o << " ]; ";
        return o;
      }
    template<typename T>
      std::ostream&
      operator << (std::ostream& o, const polygon_set_data<T>& r)
      {
        o << "Polygon Set Data { ";
        for (typename polygon_set_data<T>::iterator_type itr = r.begin ();
            itr != r.end (); ++itr)
          {
            o << "<" << (*itr).first.first << ", " << (*itr).first.second
                << ">:" << (*itr).second << " ";
          }
        o << "} ";
        return o;
      }
  }
}
//once again we make our usage of the library generic
//and parameterize it on the polygon set type
template<typename PolygonSet>
  void
  test_polygon_set ()
  {
    using namespace gtl;
    PolygonSet ps;
    ps += rectangle_data<int> (0, 0, 10, 10);
    PolygonSet ps2;
    ps2 += rectangle_data<int> (5, 5, 15, 15);
    PolygonSet ps3;
    assign (ps3, ps * ps2);
    PolygonSet ps4;
    ps4 += ps + ps2;
    assert(area (ps4) == area (ps) + area (ps2) - area (ps3));
    assert(equivalence ((ps + ps2) - (ps * ps2), ps ^ ps2));
    rectangle_data<int> rect;
    assert(extents (rect, ps ^ ps2));
    assert(area (rect) == 225);
    assert(area (rect ^ (ps ^ ps2)) == area (rect) - area (ps ^ ps2));

  }

void
test_cout ()
{
  using namespace gtl;

  polygon_set_data<int> psd;
  rectangle_data<int> rect;
  std::vector<polygon_data<int> > pv;

  psd.get (pv);
  for (int i = 0; i < pv.size (); ++i)
    {
      std::cout << pv[i] << std::endl;
    }

  psd -= 10;
  pv.clear ();
  psd.get (pv);
  for (int i = 0; i < pv.size (); ++i)
    {
      std::cout << pv[i] << std::endl;
    }
}

//first thing is first, lets include all the code from previous examples

//the CPoint example
struct CPoint
{
  int x;
  int y;
};

namespace boost
{
  namespace polygon
  {
    template<>
      struct geometry_concept<CPoint>
      {
        typedef point_concept type;
      };
    template<>
      struct point_traits<CPoint>
      {
        typedef int coordinate_type;

        static inline coordinate_type
        get (const CPoint& point, orientation_2d orient)
        {
          if (orient == HORIZONTAL)
            return point.x;
          return point.y;
        }
      };

    template<>
      struct point_mutable_traits<CPoint>
      {
        typedef int coordinate_type;

        static inline void
        set (CPoint& point, orientation_2d orient, int value)
        {
          if (orient == HORIZONTAL)
            point.x = value;
          else
            point.y = value;
        }
        static inline CPoint
        construct (int x_value, int y_value)
        {
          CPoint retval;
          retval.x = x_value;
          retval.y = y_value;
          return retval;
        }
      };
  }
}

//the CPolygon example
typedef std::list<CPoint> CPolygon;

//we need to specialize our polygon concept mapping in boost polygon
namespace boost
{
  namespace polygon
  {
//first register CPolygon as a polygon_concept type
    template<>
      struct geometry_concept<CPolygon>
      {
        typedef polygon_concept type;
      };

    template<>
      struct polygon_traits<CPolygon>
      {
        typedef int coordinate_type;
        typedef CPolygon::const_iterator iterator_type;
        typedef CPoint point_type;

        // Get the begin iterator
        static inline iterator_type
        begin_points (const CPolygon& t)
        {
          return t.begin ();
        }

        // Get the end iterator
        static inline iterator_type
        end_points (const CPolygon& t)
        {
          return t.end ();
        }

        // Get the number of sides of the polygon
        static inline std::size_t
        size (const CPolygon& t)
        {
          return t.size ();
        }

        // Get the winding direction of the polygon
        static inline winding_direction
        winding (const CPolygon& t)
        {
          return unknown_winding;
        }
      };

    template<>
      struct polygon_mutable_traits<CPolygon>
      {
        //expects stl style iterators
        template<typename iT>
          static inline CPolygon&
          set_points (CPolygon& t, iT input_begin, iT input_end)
          {
            t.clear ();
            while (input_begin != input_end)
              {
                t.push_back (CPoint ());
                gtl::assign (t.back (), *input_begin);
                ++input_begin;
              }
            return t;
          }

      };
  }
}

//OK, finally we get to declare our own polygon set type
typedef std::vector<CPolygon> CPolygonSet;

//deque isn't automatically a polygon set in the library
//because it is a standard container there is a shortcut
//for mapping it to polygon set concept, but I'll do it
//the long way that you would use in the general case.
namespace boost
{
  namespace polygon
  {
//first we register CPolygonSet as a polygon set
    template<>
      struct geometry_concept<CPolygonSet>
      {
        typedef polygon_set_concept type;
      };

//next we map to the concept through traits
    template<>
      struct polygon_set_traits<CPolygonSet>
      {
        typedef int coordinate_type;
        typedef CPolygonSet::const_iterator iterator_type;
        typedef CPolygonSet operator_arg_type;

        static inline iterator_type
        begin (const CPolygonSet& polygon_set)
        {
          return polygon_set.begin ();
        }

        static inline iterator_type
        end (const CPolygonSet& polygon_set)
        {
          return polygon_set.end ();
        }

        //don't worry about these, just return false from them
        static inline bool
        clean (const CPolygonSet& polygon_set)
        {
          return false;
        }
        static inline bool
        sorted (const CPolygonSet& polygon_set)
        {
          return false;
        }
      };

    template<>
      struct polygon_set_mutable_traits<CPolygonSet>
      {
        template<typename input_iterator_type>
          static inline void
          set (CPolygonSet& polygon_set, input_iterator_type input_begin,
               input_iterator_type input_end)
          {
            polygon_set.clear ();
            //this is kind of cheesy. I am copying the unknown input geometry
            //into my own polygon set and then calling get to populate the
            //deque
            polygon_set_data<int> ps;
            ps.insert (input_begin, input_end);
            ps.get (polygon_set);
            //if you had your own odd-ball polygon set you would probably have
            //to iterate through each polygon at this point and do something
            //extra
          }
      };
  }
}

int
main (int argc, char **argv)
{

  long long c1 = clock ();
  for (int i = 0; i < 1000; ++i)
    test_polygon_set<CPolygonSet> ();
  long long c2 = clock ();
  for (int i = 0; i < 1000; ++i)
    test_polygon_set<gtl::polygon_set_data<int> > ();
  long long c3 = clock ();
  long long diff1 = c2 - c1;
  long long diff2 = c3 - c2;
  if (diff1 > 0 && diff2)
    std::cout << "library polygon_set_data is " << float (diff1) / float (diff2)
        << "X faster than custom polygon set deque of CPolygon" << std::endl;
  else
    std::cout << "operation was too fast" << std::endl;

  test_cout ();

  return 0;
}

