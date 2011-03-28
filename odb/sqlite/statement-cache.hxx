// file      : odb/sqlite/statement-cache.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2005-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SQLITE_STATEMENT_CACHE_HXX
#define ODB_SQLITE_STATEMENT_CACHE_HXX

#include <odb/pre.hxx>

#include <map>
#include <typeinfo>

#include <odb/forward.hxx>
#include <odb/details/shared-ptr.hxx>
#include <odb/details/type-info.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/object-statements.hxx>
#include <odb/sqlite/details/export.hxx>

namespace odb
{
  namespace sqlite
  {
    class connection;

    class LIBODB_SQLITE_EXPORT statement_cache
    {
    public:
      statement_cache (connection&);

      simple_statement&
      begin_statement () const
      {
        return *begin_;
      }

      simple_statement&
      commit_statement () const
      {
        return *commit_;
      }

      simple_statement&
      rollback_statement () const
      {
        return *rollback_;
      }

      template <typename T>
      object_statements<T>&
      find ()
      {
        map::iterator i (map_.find (&typeid (T)));

        if (i != map_.end ())
          return static_cast<object_statements<T>&> (*i->second);

        details::shared_ptr<object_statements<T> > p (
          new (details::shared) object_statements<T> (conn_));

        map_.insert (map::value_type (&typeid (T), p));
        return *p;
      }

    private:
      typedef std::map<const std::type_info*,
                       details::shared_ptr<object_statements_base>,
                       details::type_info_comparator> map;

      connection& conn_;

      details::shared_ptr<simple_statement> begin_;
      details::shared_ptr<simple_statement> commit_;
      details::shared_ptr<simple_statement> rollback_;

      map map_;
    };
  }
}

#include <odb/post.hxx>

#endif // ODB_SQLITE_STATEMENT_CACHE_HXX