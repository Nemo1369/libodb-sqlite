// file      : odb/boost/date-time/mysql/gregorian-traits.hxx
// author    : Constantin Michael <constantin@codesynthesis.com>
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_BOOST_DATE_TIME_MYSQL_GREGORIAN_TRAITS_HXX
#define ODB_BOOST_DATE_TIME_MYSQL_GREGORIAN_TRAITS_HXX

#include <odb/pre.hxx>

#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <odb/core.hxx>
#include <odb/mysql/traits.hxx>
#include <odb/boost/date-time/exceptions.hxx>

namespace odb
{
  namespace mysql
  {
    template <>
    class value_traits< ::boost::gregorian::date, MYSQL_TIME, id_date>
    {
    public:
      typedef ::boost::gregorian::date date;
      typedef date value_type;
      typedef date query_type;
      typedef MYSQL_TIME image_type;

      static void
      set_value (date& v, const MYSQL_TIME& i, bool is_null)
      {
        if (is_null)
          v = date (::boost::date_time::not_a_date_time);
        else
          v = date (i.year, i.month, i.day);
      }

      static void
      set_image (MYSQL_TIME& i, bool& is_null, const date& v)
      {
        if (v.is_special ())
        {
          if (v.is_not_a_date ())
            is_null = true;
          else
            throw odb::boost::date_time::special_value ();
        }
        else
        {
          is_null = false;
          i.year = v.year ();
          i.month = v.month ();
          i.day = v.day ();
        }
      }
    };
  }
}

#include <odb/post.hxx>

#endif // ODB_BOOST_DATE_TIME_MYSQL_GREGORIAN_TRAITS_HXX
