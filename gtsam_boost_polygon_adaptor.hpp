/* ----------------------------------------------------------------------------
 * gtsam_boost_polygon_adaptor.hpp

 *  Created on: Jul 4, 2017
 *      Author: ivizzo
 * -------------------------------------------------------------------------- */

/**
 * @brief This adaptor is intended to teach
 * the boost.polygon library how to deal with
 * the gtsam polygons.
 *
 * Including this module in your program you will
 * be able to use the methods of the boost.polygon
 * library with the gtsam polygons.
 *
 * As a standard adaptor we need to include the template
 * library and the  "user" library where the polygon are
 * defined as well
 */

#ifndef GTSAM_BOOST_POLYGON_ADAPTOR_HPP_
#define GTSAM_BOOST_POLYGON_ADAPTOR_HPP_

#include <boost/polygon/polygon.hpp>
#include <gtsam/geometry/Pose2.h>
#include <vector>
#include <iostream>
namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

/**
 * This declarations will be used only for
 * testing purposes and in order to solve the
 * compilation error.
 * Once the compilation error is solved we
 * should clean the code and go back to
 * use gtsam::Pose2.
 *
 * @brief If we try to map the gtsam_polygon_set
 * concept to a booost::polygon::polygon_set concept
 * we will get a compilation error. We don't have
 * a solution for this yet.
 * The error is :
 *  error: no match for ‘operator=’
 *  (operand types are ‘gtsam::Point2’ and
 *  ‘const boost::polygon::point_data<double>’)
 *  in /usr/include/c++/7/bits/stl_algobase.h:294:16:
 *  *__result = *__first;
 *  ~~~~~~~~~~^~~~~~~~~~
 *
 * This seems to indicate that the Point concept is
 * not mapped properly.
 *
 * Because is easier to debug and to go back and
 * forward beetween gtsam::Poin2 and point_data<double>
 * we will use that data types until we solve this issue.
 * Once we solve this we should be able to fast forward
 * to use gtsam::Pose2 data type.
 *
 * Use : comment or or uncomment the next lines
 * if both are commented the default value for
 * the concept type will be the boost::polygon
 * point_data
 */
// #define USE_GTSAM_POINT_CONCEPT
#define USE_GTSAM_POSE_CONCEPT
// #define USE_SIMPLE_POINT_CONCEPT

#ifdef USE_SIMPLE_POINT_CONCEPT
  struct SimplePoint
  {
    double x;
    double y;
  };
 /**
  * Now we need to do some ugly black magic
  * in order to make everhing compiles.
  * Because set and get methods for this
  * simple structure are different that the
  * ones implemented in gtsam. So, I will
  * try to make this KISS, but don't expect
  * to much beacuse it will be a mess
  */
  #define _x x
  #define _y y
#else
  #define _x x()
  #define _y y()
#endif

/**
 * We use a vector of Pose2, because we are interested
 * in keep the heading information. But for the geometry
 * purposes we only need the Translation information
 * of the robot. We don't discard the heading though.
 */
/// \todo re-define if we really want to do this in this way
/// of we want to specify the gtsam::Point2 as the point concept.
typedef
#ifdef USE_GTSAM_POINT_CONCEPT
    gtsam::Point2
#else
  #ifdef USE_GTSAM_POSE_CONCEPT
      gtsam::Pose2
  #else
    #ifdef USE_SIMPLE_POINT_CONCEPT
      SimplePoint
    #else
      gtl::point_data<double>
    #endif
  #endif
#endif
        gtsam_point_t;

/**
 * We define the gtsam_polygon_t data type
 * as a vector of gtsam::Pose2. Right now
 * is implemented in this way on the code.
 * If some day this change, we should change
 * only this definition
 */
typedef std::vector<gtsam_point_t> gtsam_polygon_t;

/**
 * We define the gtsam_polygon_set_t data type
 * as a vector of gtsam_polygon_t. Right now
 * is implemented in this way on the code.
 * If some day this change, we should change
 * only this definition.
 * I'm still not quite sure why we need to
 * use set of polygons in order to take advantage
 * of all of the methods implemented in the
 * library.
 */
typedef std::vector<gtsam_polygon_t> gtsam_polygon_set_t;

/**
 *  ostream operator overloads to support debugging
 */
namespace boost
{
  namespace polygon
  {
  template <class T>
  std::ostream& operator << (std::ostream& o, const point_data<T>& r)
  {
    return o << r.get(HORIZONTAL) << ";" << r.get(VERTICAL);
  }
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
        o << "Polygon Set Data { " << std::endl;
        for (typename polygon_set_data<T>::iterator_type itr = r.begin ();
            itr != r.end (); ++itr)
          {
            o << "<" << (*itr).first.first << ", " << (*itr).first.second
                << ">: " << (*itr).second << std::endl;
          }
        o << "} ";
        return o;
      }
  }
}

std::ostream &
operator<<(std::ostream &o, const gtsam_polygon_set_t &r)
{
  o << "gtsam_polygon_set_t = [ " << std::endl;
  for (unsigned int i = 0; i < r.size(); i++)
    for (unsigned int j = 0; j < r[i].size(); j++)
      std::cout << r[i][j]._x << "," << r[i][j]._y << ";" << std::endl;
  o << "]; ";
  return o;
}

/**
 * PointType template specialization
 * <boost/polygon/point_traits.hpp>
 */
namespace boost
{
  namespace polygon
  {
    template<>
      struct geometry_concept<gtsam_point_t>
      {
        typedef point_concept type;
      };

    template<>
      struct point_traits<gtsam_point_t>
      {
        typedef double coordinate_type;

        static inline coordinate_type
        get (const gtsam_point_t &point, orientation_2d orient)
        {
          if (orient == HORIZONTAL)
            return point._x;
          else
            return point._y;
        }
      };

// ToDo : check if the set method is working
    template<>
      struct point_mutable_traits<gtsam_point_t>
      {
        typedef double coordinate_type;

        /// \todo This doesn't seems to be the best way to do this
        ///  we are constructing a new point, using old data....
        ///  I don't know, it doesn't sounds good. Oh! I almost
        ///  forgot.We are using data from the same place in memmory
        ///  to re-create it and putting it again on the same place(?)
        static inline void
        set(gtsam_point_t &point, orientation_2d orient, coordinate_type value)
        {
          if (orient == HORIZONTAL)
          {
            #ifndef USE_SIMPLE_POINT_CONCEPT
              #ifdef USE_GTSAM_POSE_CONCEPT
                point = gtsam_point_t(value, point.y(), point.theta());
              #else
                point = gtsam_point_t(value, point.y());
              #endif
            #else
                point._x = value;
            #endif
          }
          else
          {
            #ifndef USE_SIMPLE_POINT_CONCEPT
              #ifdef USE_GTSAM_POSE_CONCEPT
                point = gtsam_point_t(point.x(), value, point.theta());
              #else
                point = gtsam_point_t(point.x(), value);
              #endif
            #else
                point._y = value;
            #endif
          }
        }
        static inline gtsam_point_t
        construct(coordinate_type x, coordinate_type y)
        {
          // we put 0 because if we will be
          // dealing with the polygons inside
          // the boost.polygon library, we really
          // don't care about the heading. And if
          // we are using this constructor, is because
          // we are building a polygon to use
          // with the library.
          #ifndef USE_SIMPLE_POINT_CONCEPT
            #ifdef USE_GTSAM_POSE_CONCEPT
              return gtsam_point_t(x, y, 0.0);
            #else
              return gtsam_point_t(x, y);
            #endif
          #else
              gtsam_point_t retval;
              retval._x = x;
              retval._y = y;
              return retval;
          #endif
        }
      };
  } // polygon
} // boost

/**
 * Now we need to specialize our polygon concept
 * mapping in boost polygon:
 * <boost/polygon/polygon_traits.hpp>
 */
namespace boost
{
  namespace polygon
  {
    //first register gtsam_polygon_t as a polygon_concept type
    template <>
    struct geometry_concept<gtsam_polygon_t>
    {
      typedef polygon_concept type;
    };

    template <>
    struct polygon_traits<gtsam_polygon_t>
    {
      typedef double coordinate_type;
      typedef gtsam_polygon_t::const_iterator iterator_type;
      typedef gtsam_point_t point_type;

      // Get the begin iterator
      static inline iterator_type
      begin_points(const gtsam_polygon_t &t)
      {
        return t.begin();
      }

      // Get the end iterator
      static inline iterator_type
      end_points(const gtsam_polygon_t &t)
      {
        return t.end();
      }

      // Get the number of sides of the polygon
      static inline std::size_t
      size(const gtsam_polygon_t &t)
      {
        return t.size();
      }

      // Get the winding direction of the polygon
      static inline winding_direction
      winding(const gtsam_polygon_t &t)
      {
        return unknown_winding;
      }
    };

    template <>
    struct polygon_mutable_traits<gtsam_polygon_t>
    {
      //expects stl style iterators
      template <typename iT>
      static inline gtsam_polygon_t &
      set_points(gtsam_polygon_t &t, iT input_begin, iT input_end)
      {
        t.clear();
        while (input_begin != input_end)
        {
          t.push_back(gtsam_point_t());
          gtl::assign(t.back(), *input_begin);
          ++input_begin;
        }
        return t;
      }
    };
  }
}
#endif /* GTSAM_BOOST_POLYGON_ADAPTOR_HPP_ */
