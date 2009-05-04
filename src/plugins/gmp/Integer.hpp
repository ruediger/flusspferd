// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:
/*
Copyright (c) 2008 Rüdiger Sonderfeld

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

#ifndef FLUSSPFERD_PLUGINS_GMP_INTEGER_HPP
#define FLUSSPFERD_PLUGINS_GMP_INTEGER_HPP

#include "flusspferd/class.hpp"
#include "flusspferd/create.hpp"
#include <gmpxx.h>

namespace multi_precission {
struct Integer : public flusspferd::native_object_base {
  struct class_info : flusspferd::class_info {
    static char const *constructor_name() {
      return "Integer";
    }

    static char const *full_name() {
      return "Integer";
    }

    static object create_prototype() {
      flusspferd::object proto = flusspferd::create_object();
      create_native_method(proto, "fits_int", &Integer::fits_int);
      create_native_method(proto, "get_int", &Integer::get_int);
      create_native_method(proto, "toInt", &Integer::get_int);
      create_native_method(proto, "get_double", &Integer::get_double);
      create_native_method(proto, "toDouble", &Integer::get_double);
      create_native_method(proto, "get_string", &Integer::get_string);
      create_native_method(proto, "toString", &Integer::get_string);
      create_native_method(proto, "get_string_base", &Integer::get_string_base);
      create_native_method(proto, "sqrt", &Integer::sqrt);
      create_native_method(proto, "sgn", &Integer::sgn);
      create_native_method(proto, "abs", &Integer::abs);
      create_native_method(proto, "cmp", &Integer::cmp);
      create_native_method(proto, "add", &Integer::add);
      create_native_method(proto, "sub", &Integer::sub);
      create_native_method(proto, "mul", &Integer::mul);
      create_native_method(proto, "div", &Integer::div);
      return proto;
    }
  };
  mpz_class mp;

  Integer(flusspferd::object const &self, mpz_class const &mp);
  Integer(flusspferd::object const &self, flusspferd::call_context &x);

  bool fits_int() /*const*/;
  int get_int() /*const*/;
  double get_double() /*const*/;
  std::string get_string() /*const*/;
  std::string get_string_base(int base) /*const*/;

  template<typename T>
  static Integer &create_integer(T mp) {
    return flusspferd::create_native_object<Integer>(object(), mpz_class(mp));
  }

  // this should be external but js doesn't support overloading!
  Integer &sqrt() /*const*/;
  int sgn() /*const*/;
  Integer &abs() /*const*/;

  // operators
  void cmp(flusspferd::call_context &x) /*const*/;

  void add(flusspferd::call_context &x) /*const*/;
  void sub(flusspferd::call_context &x) /*const*/;
  void mul(flusspferd::call_context &x) /*const*/;
  void div(flusspferd::call_context &x) /*const*/;
};
}

#endif