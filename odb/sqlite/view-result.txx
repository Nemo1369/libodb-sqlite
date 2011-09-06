// file      : odb/sqlite/view-result.txx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#include <odb/callback.hxx>
#include <odb/exceptions.hxx>

#include <odb/sqlite/view-statements.hxx>

namespace odb
{
  namespace sqlite
  {
    template <typename T>
    result_impl<T, class_view>::
    ~result_impl ()
    {
    }

    template <typename T>
    result_impl<T, class_view>::
    result_impl (const query& q,
                 details::shared_ptr<select_statement> statement,
                 view_statements<view_type>& statements)
        : base_type (statements.connection ().database ()),
          result_impl_base (q, statement),
          statements_ (statements)
    {
    }

    template <typename T>
    void result_impl<T, class_view>::
    load (view_type& view)
    {
      // The image can grow between calls to load() as a result of other
      // statements execution.
      //
      typename view_traits::image_type& im (statements_.image ());

      if (im.version != statements_.image_version ())
      {
        binding& b (statements_.image_binding ());
        view_traits::bind (b.bind, im);
        statements_.image_version (im.version);
        b.version++;
      }

      select_statement::result r (statement_->load ());

      if (r == select_statement::truncated)
      {
        if (view_traits::grow (im, statements_.image_truncated ()))
          im.version++;

        if (im.version != statements_.image_version ())
        {
          binding& b (statements_.image_binding ());
          view_traits::bind (b.bind, im);
          statements_.image_version (im.version);
          b.version++;
          statement_->reload ();
        }
      }

      odb::database& db (this->database ());

      view_traits::callback (db, view, callback_event::pre_load);
      view_traits::init (view, im);
      view_traits::callback (db, view, callback_event::post_load);
    }

    template <typename T>
    void result_impl<T, class_view>::
    next ()
    {
      this->current (pointer_type ());

      if (!statement_->next ())
        this->end_ = true;
    }

    template <typename T>
    void result_impl<T, class_view>::
    cache ()
    {
    }

    template <typename T>
    std::size_t result_impl<T, class_view>::
    size ()
    {
      throw result_not_cached ();
    }
  }
}
