// -*- Mode: C++ -*-
//
// Copyright (C) 2016 Red Hat, Inc.
//
// This file is part of the GNU Application Binary Interface Generic
// Analysis and Instrumentation Library (libabigail).  This library is
// free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the
// Free Software Foundation; either version 3, or (at your option) any
// later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this program; see the file COPYING-LGPLV3.  If
// not, see <http://www.gnu.org/licenses/>.
//
// Author: Dodji Seketeli

/// @file
///
/// Declaration of types pertaining to the interned string pool used
/// throughout Libabigail, for performance reasons.
///
/// For the record, the concept of the String Interning method is
/// explained at https://en.wikipedia.org/wiki/String_interning.

#ifndef __ABG_INTERNED_STR_H__
#define __ABG_INTERNED_STR_H__

#include <tr1/memory>
#include <tr1/functional>
#include <ostream>
#include <string>

using std::tr1::shared_ptr;

namespace abigail
{

/// The abstraction of an interned string.
///
/// It's a wrapper around a pointer to a std::string, along with a set
/// of method that helps make this string integrate with std::string
/// seamlessly.  For instance, the type provides equality operators
/// that help compare it against std::string.
///
/// Note that this @ref interned_string type is design to have the
/// same size as a pointer to a string.
class interned_string
{
  std::string* raw_;

  /// Constructor.
  ///
  /// @param raw the pointer to string that this interned_string
  /// wraps.
  interned_string(std::string* raw)
    : raw_(raw)
  {}

public:

  /// Default constructor.
  ///
  /// Constructs an empty pointer to string.
  interned_string()
    : raw_()
  {}

  /// Copy constructor.
  ///
  /// @param o the other instance to copy from.
  interned_string(const interned_string& o)
  {raw_ = o.raw_;}

  /// Test if the current instance of @ref interned_string is empty.
  ///
  /// @return true iff the currentisntance of @ref interned_string is
  /// empty.
  bool
  empty() const
  {return !raw_;}

  /// Return the underlying pointer to std::string that this
  /// interned_string wraps.
  ///
  /// @return a pointer to the underlying std::string, or 0 if this
  /// interned_string is empty.
  const std::string*
  raw() const
  {return raw_;}

  /// Compare the current instance of @ref interned_string against
  /// another instance of @ref interned_string.
  ///
  /// Note that this comparison is done in O(1), because it compares
  /// the pointer values of the two underlying pointers to std::string
  /// held by each instances of @ref interned_string.
  ///
  /// @param o the other @ref interned_string to compare against.
  ///
  /// @return true iff the current instance equals @p o.
  bool
  operator==(const interned_string& o) const
  {return raw_ == o.raw_;}

  /// Inequality operator.
  ///
  /// @param o the other @ref interned_string to compare the current
  /// instance against.
  ///
  /// @return true iff the current instance is different from the @p
  /// o.
  bool
  operator!=(const interned_string& o) const
  {return !operator==(o);}

  /// Compare the current instance of @ref interned_string against
  /// an instance of std::string.
  ///
  /// Note that this comparison is done in O(N), N being the size (in
  /// number of characters) of the strings being compared.
  ///
  /// @param o the instance of std::string to compare against.
  ///
  /// @return true iff the current instance equals @p o.
  bool
  operator==(const std::string& o) const
  {
    if (raw_)
      return *raw_ == o;
    return o.empty();
  }

  /// Inequality operator.
  ///
  /// Takes the current instance of @ref interned_string and an
  /// instance of std::string.
  ///
  /// @param o the instance of std::string to compare the current
  /// instance of @ref interned_string against.
  ///
  /// @return true if the current instance of @ref interned_string is
  /// different from @p o.
  bool
  operator!=(const std::string& o) const
  {return ! operator==(o);}

  /// "Less than" operator.
  ///
  /// Lexicographically compares the current instance of @ref
  /// interned_string against another instance.
  ///
  /// @param o the other instance of @ref interned_string to compare
  /// against.
  ///
  /// @return true iff the current instance of interned_string is
  /// lexicographycally less than the string @p o.
  bool
  operator<(const interned_string& o) const
  {return static_cast<std::string>(*this) < static_cast<std::string>(o);}

  /// Conversion operator to string.
  ///
  /// @return the underlying string this instance refers too.
  operator std::string() const
  {
    if (!raw_)
      return "";
    return *raw_;
  }

  friend class interned_string_pool;
}; // end class interned_string

bool
operator==(const std::string& l, const interned_string& r);

bool
operator!=(const std::string& l, const interned_string& r);

std::ostream&
operator<<(std::ostream& o, const interned_string& s);

std::string
operator+(const interned_string& s1,const std::string& s2);

std::string
operator+(const std::string& s1, const interned_string& s2);

/// A functor to hash instances of @ref interned_string.
struct hash_interned_string
{
  /// The hash operator.
  ///
  /// It's super fast because hashing an interned string amounts to
  /// hashing the pointer to it's underlying string.  It's because
  /// every distinct string is present only in one copy in the
  /// environment.
  ///
  /// @param s the instance of @ref interned_string to hash.
  ///
  /// @return the returned hash value.
  size_t
  operator()(const interned_string& s) const
  {
    std::tr1::hash<size_t> hash_size_t;
    return hash_size_t(reinterpret_cast<size_t>(s.raw()));
  }
}; // end struct hash_interned_string


/// The interned string pool.
///
/// This is where all the distinct strings represented by the interned
/// strings leave.  The pool is the actor responsible for creating
/// interned strings.
class interned_string_pool
{
  struct priv;
  typedef shared_ptr<priv> priv_sptr;

  priv_sptr priv_;

public:

  interned_string_pool();

  interned_string
  create_string(const std::string&);

  bool
  has_string(const char* s) const;

  const char*
  get_string(const char* s) const;

  ~interned_string_pool();
}; // end class interned_string_pool

} // end namespace abigail

#endif // __ABG_INTERNED_STR_H__
