/*
 * rosam_boost_adaptor.hpp
 *
 *  Created on: Jul 4, 2017
 *      Author: ivizzo
 */

#ifndef ROSAM_BOOST_ADAPTOR_HPP_
#define ROSAM_BOOST_ADAPTOR_HPP_


/**
 * @brief This adaptor is intended to teach
 * the boost.polygon library how to deal with
 * the rosam polygons.
 *
 * Including this module in your program you will
 * be able to use the methods of the boost.polygon
 * library with the rosam polygons.
 *
 * As a standard adaptor we need to include the template
 * library and the  "user" library where the polygon are
 * defined as well
 */
#include <boost/polygon/polygon.hpp>
#include <gtsam/geometry/Pose2.h>

/**
 * We use a vector of Pose2, because we are interested
 * in keep the heading information. But for the geometry
 * purposes we only need the Translation information
 * of the robot. We don't discard the heading though.
 */
//typedef gtsam::Pose2::Translation rosam_point_t;
/// \todo re-define if we really want to do this in this way
/// of we want to specify the gtsam::Point2 as the point concept.
typedef gtsam::Pose2 rosam_point_t;

/**
 * We define the rosam_polygon_t data type
 * as a vector of gtsam::Pose2. Right now
 * is implemented in this way on the code.
 * If some day this change, we should change
 * only this definition
 */
#include <vector>
typedef std::vector<rosam_point_t> rosam_polygon_t;

namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

/**
 * PointType template specialization
 * <boost/polygon/point_traits.hpp>
 */
namespace boost
{
  namespace polygon
  {
    template<>
      struct geometry_concept<rosam_point_t>
      {
        typedef point_concept type;
      };

    template<>
      struct point_traits<rosam_point_t>
      {
        typedef double coordinate_type;

        static inline coordinate_type
        get (const rosam_point_t &point, orientation_2d orient)
        {
          if (orient == HORIZONTAL)
            return point.x ();
          else
            return point.y ();
        }
      };

// ToDo : check if the set method is working
    template<>
      struct point_mutable_traits<rosam_point_t>
      {
        typedef double coordinate_type;

        /// \todo This doesn't seems to be the best way to do this
        ///  we are constructing a new point, using old data....
        ///  I don't know, it doesn't sounds good. Oh! I almost
        ///  forgot.We are using data from the same place in memmory
        ///  to re-create it and putting it again on the same place(?)
        static inline void
        set (rosam_point_t &point, orientation_2d orient, coordinate_type value)
        {
          if (orient == HORIZONTAL)
            {
              point = rosam_point_t (value, point.y (), point.theta ());
            }
          else
            {
              point = rosam_point_t (point.x (), value, point.theta ());
            }
        }
        static inline rosam_point_t
        construct (coordinate_type x, coordinate_type y)
        {
          // we put 0 because if we will be
          // dealing with the polygons inside
          // the boost.polygon library, we really
          // don't care about the heading. And if
          // we are using this constructor, is because
          // we are building a polygon to use
          // with the library.
          return rosam_point_t (x, y, 0.0);
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
//first register rosam_polygon_t as a polygon_concept type
    template<>
      struct geometry_concept<rosam_polygon_t>
      {
        typedef polygon_concept type;
      };

    template<>
      struct polygon_traits<rosam_polygon_t>
      {
        typedef double coordinate_type;
        typedef rosam_polygon_t::const_iterator iterator_type;
        typedef rosam_point_t point_type;

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

/**
 * We could use the std::vector or std::list
 * specialization that it's already written.
 * But because there is an issue with the get
 * method(basically it can't construct the
 * gstam::Pose2 based polygon) we need to
 * write our own specialization.
 */

#include <iostream>
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


//OK, finally we get to declare our own polygon set type
typedef std::vector<rosam_polygon_t> rosam_polygon_set_t;
std::ostream &
operator<<(std::ostream &o, const rosam_polygon_set_t &r)
{
  o << "rosam_polygon_set_t = [ " << std::endl;
  for (unsigned int i = 0; i < r.size(); i++)
    for (unsigned int j = 0; j < r[i].size(); j++)
      std::cout << r[i][j].x() << "," << r[i][j].y() << ";" << std::endl;
  o << "]; ";
  return o;
}
//deque isn't automatically a polygon set in the library
//because it is a standard container there is a shortcut
//for mapping it to polygon set concept, but I'll do it
//the long way that you would use in the general case.
namespace boost
{
  namespace polygon
  {
//first we register rosam_polygon_set_t as a polygon set
    template<>
      struct geometry_concept<rosam_polygon_set_t>
      {
        typedef polygon_set_concept type;
      };

//next we map to the concept through traits
    template<>
      struct polygon_set_traits<rosam_polygon_set_t>
      {
        typedef double coordinate_type;
        typedef rosam_polygon_set_t::const_iterator iterator_type;
        typedef rosam_polygon_set_t operator_arg_type;

        static inline iterator_type
        begin (const rosam_polygon_set_t &polygon_set)
        {
          return polygon_set.begin ();
        }

        static inline iterator_type
        end (const rosam_polygon_set_t &polygon_set)
        {
          return polygon_set.end ();
        }

        //don't worry about these, just return false from them
        static inline bool
        clean (const rosam_polygon_set_t &polygon_set)
        {
          return false;
        }
        static inline bool
        sorted (const rosam_polygon_set_t &polygon_set)
        {
          return false;
        }
      };

    template<>
      struct polygon_set_mutable_traits<rosam_polygon_set_t>
      {
      template <typename input_iterator_type>
          static inline void set(rosam_polygon_set_t& polygon_set, input_iterator_type input_begin, input_iterator_type input_end) {
            polygon_set.clear();
            polygon_set_data<double> psd;
            psd.insert(input_begin, input_end);

            std::cout << psd << std::endl;

            // now we have our data copied into
            // the psd polygon set. We need to
            // copy that data into the user defined
            // polygon.
            /// \todo : Find the way to do this in a
            /// propper manner. We would like to write:
            /// -> psd.get(polygon_set);
            typedef polygon_set_data<double>::iterator_type iterator_type;
            for (iterator_type itr = psd.begin(); itr != psd.end(); ++itr)
            {
              std::cout << "Fill your rosam_polygon_t here..."
                        << std::endl;
            }
      }
      };
  }
}


#endif /* ROSAM_BOOST_ADAPTOR_HPP_ */
