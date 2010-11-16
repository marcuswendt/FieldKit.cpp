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
 * compiler needs all the implementation visible to expand it.
 */

#include <string.h>

//-----------------------------------------------------------------------------
#include "fieldkit/script/cproxyv8/cproxyv8-function.h"
#include "fieldkit/script/cproxyv8/cproxyv8-property.h"
//-----------------------------------------------------------------------------

/** 
* Handy macro to get the proxy class
*
* \code
*    ProxyClass<Point>* cPoint = CPROXYV8_CLASS(Point);
* \endcode
*/
#define CPROXYV8_CLASS(CLASS) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#CLASS"")

/** 
* Handy macro to get the proxy class
*
* \code
*    ProxyClass<Point>* cPoint = CPROXYV8_CLASS(Point);
* \endcode
*/
#define CPROXYV8_CLASS_ID(CLASS,ID) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#ID"")


/** 
* Handy macro to expose properties of the ProxyClass<T> with the same ID in JS
*
* \code
*    CPROXYV8_PROPERTY(Point, x, true, true);
* \endcode
*/
#define CPROXYV8_PROPERTY(CLASS, PROPERTY, GET, SET) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#CLASS""); \
  CProxyV8::ProxyClass<CLASS>::instance()->Expose(&CLASS::PROPERTY, ""#PROPERTY"", SET, GET)

/** 
* Handy macro to expose properties of the ProxyClass<T> with different ID in JS
*
* \code
*    CPROXYV8_PROPERTY_ID(Point, x, x_, true, true);
* \endcode
*/

#define CPROXYV8_PROPERTY_ID(CLASS, PROPERTY, ID, GET, SET) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#CLASS""); \
  CProxyV8::ProxyClass<CLASS>::instance()->Expose(&CLASS::PROPERTY, ""#ID"", SET, GET)

/** 
* Handy macro to expose methods of the ProxyClass<T> with the same ID in JS
*
* \code
*    CPROXYV8_METHOD(Point, translate);
* \endcode
*/
#define CPROXYV8_METHOD(CLASS, METHOD) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#CLASS""); \
  CProxyV8::ProxyClass<CLASS>::instance()->Expose(&CLASS::METHOD, ""#METHOD"")

/**
* Handy macro to expose methods of the ProxyClass<T> with different ID in JS
*
* \code
*    CPROXYV8_METHOD_ID(Point, Translate, translate);
* \endcode
*/
#define CPROXYV8_METHOD_ID(CLASS, METHOD, ID) \
  CProxyV8::ProxyClass<CLASS>::instance()->init(""#CLASS""); \
  CProxyV8::ProxyClass<CLASS>::instance()->Expose(&CLASS::METHOD, ""#ID"")

namespace CProxyV8
{
  using namespace v8;
/**
 * Type::Cast is a facade pattern, and provides a simplified interface between C++ and 
 *            JavaScript.
 *
 * This encapsulate the cast between objects from JS->C++ and from C++->JS
 *
 * The only down side of this is when casting from JS to C++, it requires pointer
 * to the target type, this is necessary to give to the compiler the information about 
 * witch Cast function needs to be call.
 *
 * If a new primitive needs to be added here is where the cast need to be added.
 */
namespace Type
{
  /** Constructor C++ */
  template <class T>
  inline T* New(T*t)
  { 
    ASSERT(!t);
    return new T();
  }

  /** Cast from V8 to C++ */
  inline bool Cast(bool*, Local<Value> value) 
  { 
    return value->BooleanValue(); 
  }	

  inline double Cast(double*, Local<Value> value) 
  { 
    return value->NumberValue(); 
  }	

  inline uint32_t Cast(uint32_t*, Local<Value> value) 
  { 
    return value->Uint32Value(); 
  }	

  inline int32_t  Cast(int32_t*, Local<Value> value) 
  { 
    return value->Int32Value(); 
  }

  inline int64_t  Cast(int64_t*, Local<Value> value) 
  { 
    return value->IntegerValue(); 
  }

  inline Handle<Value> Cast(Handle<Value> value) 
  { 
    return value;
  };


  /** Cast V8:Value to primitive value */
  template<class T>
  inline T Cast(T*, Local<Value> value) 
  { 
    return ProxyClass<T>::instance()->UnwrapObject(value);
  }

  template<class T>
  inline T Cast(T&, Local<Value> value) 
  { 
    return ProxyClass<T>::instance()->UnwrapObject(value);
  }

  /** Cast from C++ to V8 */
  inline Handle<Value> Cast(bool caseParam)     { return Boolean::New(caseParam); }	
  inline Handle<Value> Cast(double caseParam)   { return Number::New(caseParam); }  
  inline Handle<Value> Cast(uint32_t caseParam) { return Uint32::New(caseParam); }	
  inline Handle<Value> Cast(int32_t  caseParam) { return Int32::New(caseParam); }

  template<class T>
  inline Handle<Value> Cast(T* t) 
  { 
    return ProxyClass<T>::instance()->ExposeObject(t, false);
  };

  template<class T>
  inline Handle<Value> Cast(T& t) 
  { 
    return ProxyClass<T>::instance()->ExposeObject(&t, false);
  };

  /** Function return value Cast from C++ to V8 */
  inline Handle<Value> FCast(bool caseParam)     { return Cast(caseParam); }	
  inline Handle<Value> FCast(double caseParam)   { return Cast(caseParam); }  
  inline Handle<Value> FCast(uint32_t caseParam) { return Cast(caseParam); }	
  inline Handle<Value> FCast(int32_t  caseParam) { return Cast(caseParam); }

  template<class T>
  inline Handle<Value> FCast(T* t) 
  { 
    return ProxyClass<T>::instance()->ExposeObject(t, true);
  };

  template<class T>
  inline Handle<Value> FCast(T& t) 
  { 
    return ProxyClass<T>::instance()->ExposeObject(&t, true);
  };

};



//-----------------------------------------------------------------------------
//-------------------------------------------------------------------- Property
//-----------------------------------------------------------------------------

template <class T>
class InstaceHandle
{
public:
  InstaceHandle(bool toBeCreated)
    : t_(NULL), toBeDestroy_(toBeCreated),toBeCreated_(toBeCreated) {};

  explicit InstaceHandle(T* t) 
    : t_(t), toBeDestroy_(false), toBeCreated_(false) {}

  ~InstaceHandle() { if (t_ && toBeDestroy_) delete t_; }

  inline void set(T*t, bool toBeDestroy);

  /**
   * Returns true if the handle is empty.
   */
  inline bool IsEmpty();

  T* operator->();

  T* operator*();

  /**
   * Checks whether two handles are the same.
   * Returns true if both are empty, or if the objects
   * to which they refer are identical.
   * The handles' references are not checked.
   */
  template <class S> bool operator==(Handle<S> that);

  /**
   * Checks whether two handles are different.
   * Returns true if only one of the handles is empty, or if
   * the objects to which they refer are different.
   * The handles' references are not checked.
   */
  template <class S> bool operator!=(Handle<S> that) 
  {
    return !operator==(that);
  }

  static InstaceHandle<T>* Cast(void* that) 
  {
    return static_cast<InstaceHandle<T>*>(that);
  }

private:
  T* t_;
  bool toBeDestroy_;
  bool toBeCreated_;
};

template <class T>
T* InstaceHandle<T>::operator->()
{
  if (!t_ && toBeCreated_)
  {
    t_ = Type::New((T*)NULL);
    toBeCreated_ = false;
  }

  return t_;
}

template <class T>
void InstaceHandle<T>::set(T*t, bool toBeDestroy)
{ 
  if (t_ && toBeDestroy_) 
    delete t_; 
  toBeDestroy = toBeDestroy; 
  t_ = t;
}

template <class T>
template <class S>
bool InstaceHandle<T>::operator==(Handle<S> that)
{
  if (!t_ && toBeCreated_)
    return false;

  void** a = reinterpret_cast<void**>(**this);
  void** b = reinterpret_cast<void**>(*that);
  if (a == 0) return b == 0;
  if (b == 0) return false;
  return *a == *b;
}


template <class T>
bool InstaceHandle<T>::IsEmpty() 
{ 
    return t_ == 0 && !toBeCreated_; 
}

template <class T>
T* InstaceHandle<T>::operator*()
{
  if (!t_ && toBeCreated_)
  {
    t_ = Type::New((T*)NULL);
    toBeCreated_ = false;
  }

  return t_;
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------- class
//-----------------------------------------------------------------------------

/** 
 * The ProxyClass<T> have the following objectives:
 *
 *    1. Provide an EASY way to expose properties from Cpp objects to JS
 *    2. Provide an EASY way to expose methods from Cpp objects to JS
 *    3. Avoid the need of MACROS to declare types or keep tracking of all
 *       classes. The compiler must take care of this with the templates.
 *    4. Avoid the need of specifics callbacks declarations required by 
 *       V8 and replace it by hidden generic callbacks directly to the 
 *       object instance.
 *    5. Achive this goals without modification of V8 code.
 *
 * ProxyClass<T> is implemented with cpp templates classes.
 * 
 * Every ProxyClass<T> is a singleton instance. The compiler handles
 * the resources allocation for the specific class instance. Making the 
 * ProxyClass<T> usage scope independent. There is another avantage,
 * the singlenton restriction force the program to have only 1 representation
 * of the specific ProxyClass<T> in C++ and JS, this avoid confussion
 * and force good code practices.
 *
 * ProxyClass<T> does not keep any references to objects of type T.
 *
 *  IMPORTAN IMPORTANT IMPORTANT!!!!!
 *
 *  BE AWARE: There is the posibilitye to create memory problems if the 
 *            T instance is dispose and there are reference to his properties
 *            in JS. 
 *
 *  TODO: Do something to help to solve this problem or at least give a warning.
 *
 * The following example shows how to use a ProxyClass<T>:
 *
 * \code
 * class Point 
 * {
 * 	public:
 *     ** Constructor without params is required *
 * 		Point(int px = 0, int py = 0) : x_(px), y_(py) {}
 *     Point(Point* p) : x_(p->x_), y_(p->y_) {}
 * 
 * 		int x_;
 *     int y_;
 * 
 *     ** Example call to function without params *
 *     int GetManhattanLength()  { return std::abs(x_) - std::abs(y_); }
 * 
 *     ** this will create a JS object, with JS destructor *
 *     Point* Copy() { return new Point(); }
 *  
 *     ** Example call to function with params *
 *     v8::Handle<v8::Value> Translate(const v8::Arguments& args) 
 *     { 
 *       // throw if we didn't get 2 args
 *       if (args.Length() != 2)
 *         return ThrowException(v8::String::New("Expected 2 arguments"));
 * 
 *       x_ += args[0]->Int32Value();
 *       y_ += args[1]->Int32Value();
 *       return v8::True(); 
 *     };
 * };
 * 
 *  **
 *  * Simple class to show how to expose object properties and methods with
 *  * reference to this properties.
 *  *
 *  *  BE AWARE: There can create memory problems if the line instance is dispose and
 *  *            there are reference to his properties in JS. Ha handle is a better
 *  *            way to expose this objects.
 *  *
 *  *
 * class Line
 * {
 * public:
 *   ** Constructor without params is required *
 *   
 *   ** Example of object property, it can be manipulate directly *
 *   Point start;  
 * 
 *   ** this will create a JS object, without destructor and a reference to this 
 *    *  end instance 
 *    *
 *    *
 *   Point& GetEnd() { return end; }
 * 
 * private:
 *   Point end;
 * };
 *
 *    int main(int argc, char* argv[]) 
 *    {
 *      ProxyClass<Point>* classPoint = PROXY_CLASS(Point);
 *      classPoint->Expose("x", &Point::x_, true, true);
 *      classPoint->Expose("y", &Point::y_, true, true);
 *
 *      ProxyClass<Line>* classLine = PROXY_CLASS(Line);
 *      classLine->Expose("start", &Line::start_, true, true);
 *      classLine->Expose("end", &Line::end_, true, true);
 *      classLine->Expose("getXDistance", &Line::GetXDistance);
        classLine->Expose("getYDistance", &Line::GetYDistance);
 *     ....
 *
 *      v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
 *
 *      global->Set(v8::String::New("Point"), classPoint->GetFunctionTemplate()); // add Point object
 *      global->Set(v8::String::New("Line"), classLine->GetFunctionTemplate()); // add Line object
 *     ....
 *    }
 *
 *    //--- JS code
 *    > point = new Point();
 *    > line = new Line();
 *    > line.getXDistance();
 *    0
 *    > line.start.x = 10;
 *    > line.getXDistance();
 *    10
 *    >
 *
 * \endcode
 * 
 */
template<class T>
class ProxyClass
{
public:  
	/** 
   * init can be called multiple times but the name must be
   * same as the first call for this template.
   */ 
  ProxyClass<T>* init(char*name);

  /** returns the singleton instance for the template class */ 
  static ProxyClass<T>* instance() { return &proxyClass; }

  /** Expose class property member to JavaScript */ 
	template<class TProperty>
	void Expose( TProperty T::*property, char* propertyName,
               bool getter, bool setter);

  /** Expose class property member to JavaScript whit a custon accessor */ 
	template<class TProperty>
	void Expose( TProperty T::*property, char* propertyName,
               AccessorProperty* accessor, bool getter, bool setter);

  /** Expose class method member to JavaScript, can return any type and
   *  has no parameters. 
   *
   *  BE AWARE OF:
   *    IF the method returns TClass* the object WILL BE DISPOSE 
   *    when the garbage collector reclain the memory.
   *
   *    IF the method returns TClass& the object WILL BE NOT DESTROY 
   *    by the garbage collector.   
   */ 
  template<class TClass>
  void Expose(TClass (T::*function)(), char* functionName);

  void Expose(Handle<Value> (T::*Method) (const Arguments& args), char* functionName);

  void Expose(AccessorFunction* accessorFunction, char* functionName);
  
  T* UnwrapObject(Local<Object> holder);
  T* UnwrapObject(Local<Value> value);

  Handle<Value> ExposeObject(T* t, bool withDestroy);
  void ExposeObject(v8::Handle<v8::ObjectTemplate> context, T* t, const char* name, bool withDestroy);

  Handle<FunctionTemplate> GetFunctionTemplate() { return functionTemplate_; };

private:
  /** Singleton ProxyClass<T> */
	ProxyClass() {};

  /** Singleton JavaScript name */
  Local<String> className_;

  /** List of accessors, this is used to dispose accessors when the template 
   *  is destroy 
   */
  internalUtil::AccessorList accessorList_;

  /** V8 FunctionTemplate */
	Handle<FunctionTemplate> functionTemplate_;

  /** V8 ObjectTemplate */
	Handle<ObjectTemplate> objectTemplate_;

  /** static single class ProxyClass<T> */
  static ProxyClass<T> proxyClass;
};


template<class T>
ProxyClass<T> ProxyClass<T>::proxyClass;

template<class T>
ProxyClass<T>* ProxyClass<T>::init(char* name)
{
  if (!className_.IsEmpty())
  {
    String::Utf8Value currentName(className_);
    //If the name is the same just return the instance
    if (strcmp(*currentName,name)) 
    {
      Utils::ReportApiFailure("cProxyV8::ProxyClass", "Error initializing - Class already exist");  
      return NULL;
    }
  }
  else
  {
    className_ = String::New(name);

	  functionTemplate_ = FunctionTemplate::New(Create<T>);
	  functionTemplate_->SetClassName(className_);

	  // Instance template from which Point objects are based on
	  objectTemplate_ = functionTemplate_->InstanceTemplate();
	  objectTemplate_->SetInternalFieldCount(1); // create an internal field for the C++ object
  }

  return instance();
}

template<class T>
template<class TProperty>
void ProxyClass<T>::Expose( TProperty T::*property, char* propertyName,
                            bool getter, bool setter)
{
  ObjectProperty<T,TProperty>* accessor = new ObjectProperty<T,TProperty>(property);
  accessorList_.push(accessor);
  Expose(property, propertyName, accessor, getter, setter);
}

template<class T>
template<class TProperty>
void ProxyClass<T>::Expose( TProperty T::*property, char* propertyName,
                            AccessorProperty* accessor, bool getter, bool setter)
{
  ASSERT(propertyName);
  ASSERT(setter || getter);

	AccessorGetter getterAccessor = getter ? Accessor_GetProperty : 0;
	AccessorSetter setterAccessor = setter ? Accessor_SetProperty : 0;
  Handle<Value> data = External::New(accessor);

	objectTemplate_->SetAccessor(String::New(propertyName), getterAccessor, setterAccessor, data);
}
/***/

template<class T>
template<class TClass>
void ProxyClass<T>::Expose(TClass (T::*function)(), char* functionName)
{
  AccessorFunction* accessor = new FuncRetObj<T,TClass>(function);
  accessorList_.push(accessor);
  Expose(accessor, functionName);
}

template<class T>
void ProxyClass<T>::Expose(Handle<Value> (T::*function) (const Arguments& args),
                           char* functionName)
{
  AccessorFunction* accessor = new FuncV8<T>(function);
  accessorList_.push(accessor);
  Expose(accessor, functionName);
}

template<class T>
void ProxyClass<T>::Expose(AccessorFunction* accessorFunction, char* functionName)
{
  Handle<Value> data = External::New(accessorFunction);
  Local<FunctionTemplate> func = FunctionTemplate::New();
  func->SetCallHandler(Accessor_Function,data);  
  objectTemplate_->Set(functionName, func);  
}

template<class T>
T* ProxyClass<T>::UnwrapObject(Local<Object> holder)
{
  Local<External> external = Local<External>::Cast(holder->GetInternalField(0));
  InstaceHandle<T>* tHandle = InstaceHandle<T>::Cast(external->Value());
  return *(*tHandle);
}

template<class T>
T* ProxyClass<T>::UnwrapObject(Local<Value> value)
{
  Local<External> external = Local<External>::Cast(value);
  InstaceHandle<T>* tHandle = static_cast<InstaceHandle<T>*>(external->Value());
  return *(*tHandle);
}

template<class T>
void ProxyClass<T>::ExposeObject(v8::Handle<v8::ObjectTemplate> context, T* t, const char* name, bool withDestroy)
{  
  HandleScope handle_scope;
  context->Set(String::New(name), ExposeObject(t, withDestroy));
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------- 
//-----------------------------------------------------------------------------

template<class T>
Handle<Value> ProxyClass<T>::ExposeObject(T* t, bool withDestroy)
{
  // Handle scope for temporary handles.
  HandleScope handle_scope;

  Handle<ObjectTemplate> templ = objectTemplate_;

  // Create an empty http request wrapper.
  Handle<Object> result = templ->NewInstance();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Local<External> externalVal = Local<External>::Cast(result->GetInternalField(0));

  InstaceHandle<T>* tHandle = static_cast<InstaceHandle<T>*>(externalVal->Value());
  tHandle->set(t, withDestroy);

  // Return the result through the current handle scope.  Since each
  // of these handles will go away when the handle scope is deleted
  // we need to call Close to let one, the result, escape into the
  // outer handle scope.
  return handle_scope.Close(result);
}

// js T constructor function, called when `new T(...)' in js
template<class T>
Handle<Value> Create(const Arguments& args)
{
	// throw if called without `new'
	if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));

	// throw if we didn't get 0 args
	if (args.Length() != 0)
		return ThrowException(String::New("Expected no arguments"));

	// create the C++ Handle<T> to be wrapped
	InstaceHandle<T>* t = new InstaceHandle<T>(true);

	// make a persistent handle for the instance and make it
	// weak so we get a callback when it is garbage collected
	Persistent<Object> self = Persistent<Object>::New(args.Holder());
	self.MakeWeak(t, WeakReferenceCallback(Class_Destroy<T>));

	// set internal field to the C++ point 
	self->SetInternalField(0, External::New(t));

	return self;
}

/**
 * Called when the T object is being garbage collected
 * delete the C++ object and ClearWeak on the Persistent handle.
 */
template<class T>
void Class_Destroy(Persistent<Object> self, void* parameter)
{
	delete static_cast<T*>(parameter);
	self.ClearWeak();
}

}
