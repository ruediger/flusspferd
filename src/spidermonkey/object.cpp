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

#include "flusspferd/object.hpp"
#include "flusspferd/property_iterator.hpp"
#include "flusspferd/function.hpp"
#include "flusspferd/exception.hpp"
#include "flusspferd/local_root_scope.hpp"
#include "flusspferd/root.hpp"
#include "flusspferd/arguments.hpp"
#include "flusspferd/string.hpp"
#include "flusspferd/native_object_base.hpp"
#include "flusspferd/implementation/init.hpp"
#include "flusspferd/implementation/value.hpp"
#include "flusspferd/implementation/object.hpp"
#include <cassert>
#include <js/jsapi.h>

using namespace flusspferd;

object::object() : Impl::object_impl(0) { }
object::~object() { }

object object::get_parent() {
  if (!is_valid())
    throw exception("Could not get object parent (object is null)");
  return Impl::wrap_object(JS_GetParent(Impl::current_context(), get()));
}

object object::get_prototype() {
  if (!is_valid())
    throw exception("Could not get object prototype (object is null)");
  return Impl::wrap_object(JS_GetPrototype(Impl::current_context(), get()));
}

void object::set_parent(object const &o) {
  if (!is_valid())
    throw exception("Could not set parent (object is null)");
  if (!JS_SetParent(Impl::current_context(), get(), o.get_const()))
    throw exception("Could not set object parent");
}

void object::set_prototype(object const &o) {
  if (!is_valid())
    throw exception("Could not set object prototype (object is null)");
  if (!JS_SetPrototype(Impl::current_context(), get(), o.get_const()))
    throw exception("Could not set object prototype");
}

void object::set_property(char const *name, value const &v_) {
  if (!is_valid())
    throw exception("Could not set property (object is null)");
  value v = v_;
  if (!JS_SetProperty(Impl::current_context(), get(), name,
                      Impl::get_jsvalp(v)))
    throw exception("Could not set property");
}

void object::set_property(std::string const &name, value const &v) {
  set_property(name.c_str(), v);
}

void object::set_property(value const &id, value const &v_) {
  if (!is_valid())
    throw exception("Could not set property (object is null)");
  local_root_scope scope;
  value v = v_;
  string name = id.to_string();
  if (!JS_SetUCProperty(Impl::current_context(), get(),
                        name.data(), name.length(),
                        Impl::get_jsvalp(v)))
    throw exception("Could not set property");
}

value object::get_property(char const *name) const {
  if (!is_valid())
    throw exception("Could not get property (object is null)");
  value result;
  if (!JS_GetProperty(Impl::current_context(), get_const(),
                      name, Impl::get_jsvalp(result)))
    throw exception("Could not get property");
  return result;
}

value object::get_property(std::string const &name) const {
  return get_property(name.c_str());
}

value object::get_property(value const &id) const {
  if (!is_valid())
    throw exception("Could not get property (object is null)");
  value result;
  local_root_scope scope;
  string name = id.to_string();
  if (!JS_GetUCProperty(Impl::current_context(), get_const(),
                        name.data(), name.length(),
                        Impl::get_jsvalp(result)))
    throw exception("Could not get property");
  return result;
}

bool object::has_property(char const *name) const {
  if (!is_valid())
    throw exception("Could not check property (object is null)");
  JSBool foundp;
  if (!JS_HasProperty(Impl::current_context(), get_const(), name,
                      &foundp))
    throw exception("Could not check property");
  return foundp;
}

bool object::has_property(std::string const &name) const {
  return has_property(name.c_str());
}

bool object::has_property(value const &id) const {
  if (!is_valid())
    throw exception("Could not check property (object is null)");
  local_root_scope scope;
  string name = id.to_string();
  JSBool foundp;
  if (!JS_HasUCProperty(Impl::current_context(), get_const(),
                        name.data(), name.length(),
                        &foundp))
    throw exception("Could not check property");
  return foundp;
}

void object::delete_property(char const *name) {
  if (!is_valid())
    throw exception("Could not delete property (object is null)");
  if (!JS_DeleteProperty(Impl::current_context(), get(), name))
    throw exception("Could not delete property");
}

void object::delete_property(std::string const &name) {
  delete_property(name.c_str());
}

void object::delete_property(value const &id) {
  if (!is_valid())
    throw exception("Could not delete property (object is null)");
  local_root_scope scope;
  string name = id.to_string();
  jsval dummy;
  if (!JS_DeleteUCProperty2(Impl::current_context(), get(),
                            name.data(), name.length(), &dummy))
    throw exception("Could not delete property");
}

property_iterator object::begin() const {
  return property_iterator(*this);
}

property_iterator object::end() const {
  return property_iterator();
}

void object::define_property(
  string const &name, value const &init_value, unsigned flags,
  boost::optional<function const &> getter_,
  boost::optional<function const &> setter_)
{
  if (!is_valid())
    throw exception("Could not define property (object is null)");

  value v;
  v = init_value;

  function getter;
  if (getter_) getter = getter_.get();
  function setter;
  if (setter_) setter = setter_.get();

  JSObject *getter_o = Impl::get_object(getter);
  JSObject *setter_o = Impl::get_object(setter);
   
  unsigned sm_flags = 0;

  if (~flags & dont_enumerate) sm_flags |= JSPROP_ENUMERATE;
  if (flags & read_only_property) sm_flags |= JSPROP_READONLY;
  if (flags & permanent_property) sm_flags |= JSPROP_PERMANENT;
  if (flags & shared_property) sm_flags |= JSPROP_SHARED;

  if (getter_o) sm_flags |= JSPROP_GETTER;
  if (setter_o) sm_flags |= JSPROP_SETTER;

  if(!JS_DefineUCProperty(Impl::current_context(), get_const(),
                          name.data(), name.length(),
                          Impl::get_jsval(v),
                          (JSPropertyOp) getter_o,
                          (JSPropertyOp) setter_o,
                          sm_flags))
    throw exception("Could not define property");
}

void object::define_property(
  std::string const &name_, value const &init_value, unsigned flags,
  boost::optional<function const &> getter,
  boost::optional<function const &> setter)
{
  local_root_scope scope;
  string name(name_);
  define_property(name, init_value, flags, getter, setter);
}

void object::define_property(
  char const *name_, value const &init_value, unsigned flags,
  boost::optional<function const &> getter,
  boost::optional<function const &> setter)
{
  local_root_scope scope;
  string name(name_);
  define_property(name, init_value, flags, getter, setter);
}

bool object::is_valid() const {
  return get_const();
}

value object::apply(object const &fn, arguments const &arg_) {
  if (!is_valid())
    throw exception("Could not apply function (object is null)");

  value fnv(fn);
  root_value result((value()));

  arguments arg(arg_);

  JSBool status = JS_CallFunctionValue(
      Impl::current_context(),
      get(),
      Impl::get_jsval(fnv),
      arg.size(),
      arg.get(),
      Impl::get_jsvalp(result));

  if (!status)
    throw exception("Could not call function");

  return result;
}

value object::call(char const *fn, arguments const &arg_) {
  if (!is_valid())
    throw exception("Could not call function (object is null)");

  root_value result((value()));

  arguments arg(arg_);

  JSBool status = JS_CallFunctionName(
      Impl::current_context(),
      get(),
      fn,
      arg.size(),
      arg.get(),
      Impl::get_jsvalp(result));

  if (!status)
    throw exception("Could not call function");

  return result;
}

value object::call(std::string const &fn, arguments const &arg) {
  return call(fn.c_str(), arg);
}

value object::call(object o, arguments const &arg) {
  return o.apply(*this, arg);
}

value object::call(arguments const &arg) {
  return call(global(), arg);
}
