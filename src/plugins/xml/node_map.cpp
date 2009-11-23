// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:
/*
The MIT License

Copyright (c) 2008, 2009 Flusspferd contributors (see "CONTRIBUTORS" or
                                       http://flusspferd.org/contributors.txt)

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

#include <flusspferd.hpp>
#include <flusspferd/aliases.hpp>

#include "node_map.hpp"
#include "element.hpp"
#include "node.hpp"
#include "char_data.hpp"
#include "attr.hpp"

using namespace flusspferd;
using namespace flusspferd::aliases;
using namespace xml_plugin;

template <class T>
static object make_it(arabica_node &a, weak_node_map map) {
  return create<T>(
    make_vector( static_cast<typename T::wrapped_type &>(a), map )
  );
}

object node_map::create_object_from_node(arabica_node &a) {
  switch (a.getNodeType()) {
  case Arabica::DOM::Node_base::ELEMENT_NODE:
    return make_it<element>(a, shared_from_this());

  case Arabica::DOM::Node_base::TEXT_NODE:
    return make_it<text>(a, shared_from_this());

  case Arabica::DOM::Node_base::CDATA_SECTION_NODE:
    return make_it<cdata>(a, shared_from_this());

  case Arabica::DOM::Node_base::COMMENT_NODE:
    throw exception("bug in Arabica::Comment constructor");
    //return make_it<comment>(a, shared_from_this());

  case Arabica::DOM::Node_base::ATTRIBUTE_NODE:
    return make_it<attr>(a, shared_from_this());


  default:
    return create<node>( make_vector( a, shared_from_this() ) );
  };
}
