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

#ifndef FLUSSPFERD_XML_ATTRIBUTE_HPP
#define FLUSSPFERD_XML_ATTRIBUTE_HPP

#include "node.hpp"
#include "../class_description.hpp"
#include <boost/noncopyable.hpp>
#include <libxml/tree.h>

namespace flusspferd { namespace xml {

FLUSSPFERD_CLASS_DESCRIPTION(
  attribute_,
  (base, node)
  (full_name, "XML.Attribute")
  (constructor_name, "Attribute")
  (constructor_arity, 4)
  (methods,
    ("addContent", bind, add_content))
  (properties,
    ("content", getter_setter, (get_content, set_content))))
{
public:
  attribute_(object const &, call_context &);
  attribute_(object const &, xmlAttrPtr attr);
  ~attribute_();

  xmlAttrPtr c_obj() const {
    return xmlAttrPtr(node::c_obj());
  }

private:
  void init();

public: // JS methods
  void add_content(string const &);

public: // JS properties
  void set_content(value const &);
  value get_content();
};

}}

#endif
