// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:
/*
Copyright (c) 2008 Aristid Breitkreuz, Ruediger Sonderfeld

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef FLUSSPFERD_IO_FILE_CLASS_HPP
#define FLUSSPFERD_IO_FILE_CLASS_HPP

#include "stream_base.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/bool.hpp>

namespace flusspferd { namespace io {

class file_class : public stream_base {
public:
  file_class(object const &, call_context &);
  ~file_class();

  struct class_info {
    static char const *full_name() { return "IO.File"; }

    typedef boost::mpl::bool_<true> constructible;

    static char const *constructor_name() { return "File"; }
    typedef boost::mpl::size_t<1> constructor_arity;

    static object create_prototype();

    static void augment_constructor(object);
  };

private: // javascript methods
  void open(char const *name);
  void close();

private:
  class impl;
  boost::scoped_ptr<impl> p;
};

}}

#endif
