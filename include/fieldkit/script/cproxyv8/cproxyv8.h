// Copyright 2008 the CProxyV8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CPROXYV8_H_
#define CPROXYV8_H_

#include <v8.h>

template <class T>
class InstaceHandle;

template <class T>
class THandlePrimitive;

template<class T>
class ProxyClass;

class Accessor
{
};
/**
 * AccessorProperty allow the program to make the abstraction betweem the real
 * cppObj->property and the generic callback from JS
 *
 * The idea here is to take advantage of the CTemplates and the pure virtual
 * methods, to avoid the need to specify the CTemplate type in the access to the
 * cppObj->property
 *
 * This kind of integration requires args.Data() to be an implementation
 * of AccessorProperty and args.Holder() the instance of the specific object.
 *
 * This are the current supported properties:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TPrimitive cppObj->Method();   //see FuncRetPrimitive
 *      TClass cppObj->Method();       //see FuncRetObj
 *      v8::Handle<v8::Value> Method(const v8::Arguments& args);  //FuncV8
 *    ...
 * \endcode
 *
 */
class AccessorProperty : public Accessor
{
public:

  /** Changet the value of the property in the Cpp generated program */
	virtual void AccessorSetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) = 0;

  /** returns the value of the property to JS enviroment */
	virtual v8::Handle<v8::Value> AccessorGetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info) = 0;
};

/**
 * JS callback
 *
 * This function obtains the generic AccessorProperty from args,
 * and set the value of cppObj->property with the value
 */
void Accessor_SetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

/**
 * JS callback
 *
 * This function obtains the generic AccessorProperty from args,
 * and return the value of cppObj->property
 */
v8::Handle<v8::Value> Accessor_GetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info);

/**
 * AccessorFunction allow the program to make the abstraction betweem the real
 * cppObj->Method() and the generic callback from JS
 *
 * The idea here is to take advantage of the CTemplates and the pure virtual
 * methods, to avoid the need to specify the CTemplate type in the call to the
 * cppObj->Method()
 *
 * This kind of integration requires args.Data() to be an implementation
 * of AccessorFunction and args.Holder() the instance of the specific object.
 *
 * This are the current supported methods:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TPrimitive cppObj->Method();   //see FuncRetPrimitive
 *      TClass cppObj->Method();       //see FuncRetObj
 *      v8::Handle<v8::Value> Method(const v8::Arguments& args);  //FuncV8
 *    ...
 * \endcode
 *
 */
class AccessorFunction : public Accessor
{
public:

  /** Entry point from JS. */
	virtual v8::Handle<v8::Value> call(const v8::Arguments& args) = 0;
};

/**
 * JS callback
 *
 * This function obtains the generic AccessorFunction from args,
 * and call the proxy to call cppObj->method()
 */
v8::Handle<v8::Value> Accessor_Function(const v8::Arguments& args);

namespace internalUtil
{
  /**
   * Utility class to keep track of all accessors in order to be delete when 
   * the ProxyClass is disposed.
   */
  class AccessorList
  {
  public:
    AccessorList() : next_(NULL), accessor_(NULL) {}
    AccessorList(Accessor* accessor) : next_(NULL), accessor_(accessor) {}

    inline virtual ~AccessorList();
    
    inline void push(Accessor * accessor);

  private:
    Accessor* accessor_;
    AccessorList* next_;
  };


  AccessorList::~AccessorList()
  { 
    if (accessor_)
      delete accessor_; 

    if (next_)
      delete next_;
  }

  void AccessorList::push(Accessor * accessor)
  {
    AccessorList* accessorList = new AccessorList(accessor);
    accessorList = next_;
    next_ = accessorList;
  }
}
#endif