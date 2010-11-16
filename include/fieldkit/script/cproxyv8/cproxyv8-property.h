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

namespace CProxyV8
{

/**
 * ObjectProperty is the implementation of AccessorProperty that works
 * as a proxy between properties of type:
 *
 * \code
 *    class Example
 *    {
 *    public:
 *      TClass property;
 *    ...
 * \endcode
 * 
 * Where TClass represents the class of objects that already have a 
 * CProxyV8::ProxyClass, this will expose the property object to JS enviroment
 *
 *
 * issues: TClass can not be a pointer(*), you will receive a template ambiguous 
 *         type cast error if you use it.
 *
 * \code
 *    {
 *    public:
 *      Point  property; //ok
 *      Point* property; //ambiguous type
 *
 * \endcode
 *
 * Be aware that this expose the object to JS and it can be manipulated 
 * to change the state (properties values inside the returned object).
 *
 * see: CProxyV8::ProxyClass
 */
template<class T, class TProperty>
class ObjectProperty : public AccessorProperty
{
public:
	typedef TProperty T::*PropertyDir;

	ObjectProperty( PropertyDir propertyDir) 
               : propertyDir_(propertyDir) {}

	virtual void AccessorSetProperty( v8::Local<v8::String> property, 
                                    v8::Local<v8::Value> value, 
                                    const v8::AccessorInfo& info);

	virtual v8::Handle<v8::Value> AccessorGetProperty( v8::Local<v8::String> property, 
                                                     const v8::AccessorInfo& info);

  ProxyClass<T>* getTemplateClass() { return templateClass_; };

private:
	PropertyDir propertyDir_;
};

//Setter implementation for object properties
template<class T, class TProperty> 
void ObjectProperty<T,TProperty>::AccessorSetProperty( v8::Local<v8::String> property, 
                                                       v8::Local<v8::Value> value, 
                                                       const v8::AccessorInfo& info)
{
	T* t = ProxyClass<T>::instance()->UnwrapObject(info.Holder());  
  t->*propertyDir_ = Type::Cast((TProperty*) NULL, value);
}

//Getter implementation for object properties
template<class T, class TProperty> 
v8::Handle<v8::Value> ObjectProperty<T,TProperty>::AccessorGetProperty(v8::Local<v8::String> property, 
                                                                       const v8::AccessorInfo& info)
{
  T* t = ProxyClass<T>::instance()->UnwrapObject(info.Holder());
  return Type::Cast((t->*propertyDir_));
}

}