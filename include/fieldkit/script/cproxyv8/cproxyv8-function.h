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

/**
 * #if _FILE_H Directive can not be applied to templates because the 
 * compiler needs all the implementation visible to expand the templates.
 */

//---------------------------------------------------------------------

#include "fieldkit/script/cproxyv8/cproxyv8.h"

//---------------------------------------------------------------------

namespace CProxyV8
{

/**
 * TAccessorFunction is a handy class, it keeps the commom variables for the
 * specific implemtations of AccessorFunction.
 */
template<class T, class TReturn>
class TAccessorFunction : public AccessorFunction
{
public:
  typedef TReturn (T::*Function)();

  TAccessorFunction(Function function) 
                  : function_(function) {}

protected:  
  Function function_;
};

/**
 * FuncRetObj is the implementation of AccessorFunction that works
 * as a proxy between functions of type:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TClass Method();
 *    ...
 * \endcode
 * 
 * Where TClass represents the class of objects that already have a 
 * CProxyV8::ProxyClass, this will create a new jsObj of the class TClass and
 * returned to the JS enviroment.
 *
 * No arguments allowed
 *
 * issues: TClass can not be a pointer(*) or a references (&), you will
 *         receive a template ambiguous type cast error if you use it.
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      Point  Method(); //ok
 *      Point* Method(); //ambiguous type
 *      Point& Method(); //ambiguous type
 * \endcode
 *
 *         With this limitation there is an overhead that create duplicate objets,
 *         at least 2. If you need to pass complex objects use FuncV8.
 *
 * see: CProxyV8::ProxyClass
 * see: CProxyV8::FuncV8
 */

template<class T, class TClass>
class FuncRetObj : public TAccessorFunction<T, TClass>
{
public:
	typedef TClass (T::*Function)();

	FuncRetObj(Function function) 
     :  TAccessorFunction(function)
  {}

	virtual v8::Handle<v8::Value> call(const v8::Arguments& args);
};

template<class T, class TClass> 
v8::Handle<v8::Value> FuncRetObj<T,TClass>::call(const v8::Arguments& args)
{
  T* t = ProxyClass<T>::instance()->UnwrapObject(args.Holder());  
  return Type::FCast((t->*function_)());
}

/**
 * FuncV8 is the more generic implementation of the AccessorFunction it works
 * almost as the generic V8 callback but with the great advantage that you
 * receive the call inside the object instance, allowing you to access this Cpp
 * implementation more easily.
 *
 * FuncV8 works as a proxy between functions of type:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      v8::Handle<v8::Value> Method(const v8::Arguments& args);
 *    ...
 * \endcode
 *
 * The following example shows how to use a FuncV8 proxy:
 *
 * \code
 *    class Point
 *    {
 *    public:
 *        ...
 *        v8::Handle<v8::Value> setX(const v8::Arguments& args);
 *        v8::Handle<v8::Value> setY(const v8::Arguments& args);
 *        ...
 *    };
 *    ...
 *    ProxyClass<Point>* bridgePoint = NEW_BRIDGE_CLASS(Point);
 *    bridgePoint->ExposeFunc("setX", &Point::SetX);
 *    bridgePoint->ExposeFunc("setY", &Point::SetY);
 *
 *    ------------ Java Script console:
 *
 *    >x = new Point();
 *    [object]
 *    >x.setX(10);
 *    10
 * \endcode
 *
 */
typedef v8::Handle<v8::Value> V8Value;
template<class T>
class FuncV8 : public TAccessorFunction<T, V8Value>
{
public:
	typedef V8Value (T::*Function)(const v8::Arguments& args);

	FuncV8(Function function) 
    : function_(function), TAccessorFunction(0) {}

	virtual v8::Handle<v8::Value> call(const v8::Arguments& args);

private:
  Function function_;
};

template<class T> 
v8::Handle<v8::Value> FuncV8<T>::call(const v8::Arguments& args)
{
  T* t = ProxyClass<T>::instance()->UnwrapObject(args.Holder());
  return Type::Cast((t->*function_)(args));
}

//---------------------------------------------------------------------------

}