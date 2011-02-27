/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/ScriptKit_Prefix.h"
#include <v8.h>
#include <list>

// --- Prerequesites -----------------------------------------------------------
namespace fieldkit { namespace script {
	
	using namespace v8;
	using v8::Handle;
	using v8::Boolean;
	
	template<class T>
	class ProxyClass;
	
	// Helper Macros
	#define SCRIPT_CLASS(CLASS) \
		fk::script::ProxyClass<CLASS>::Instance()->init(""#CLASS"")
		
	//#define SCRIPT_PROPERTY_ID(CLASS, PROPERTY, ID, GET, SET) \
//		fk::script::ProxyClass<CLASS>::Instance()->init(""#CLASS""); \
//		fk::script::ProxyClass<CLASS>::Instance()->expose(&CLASS::PROPERTY23, ""#ID"", SET, GET)
//	
	#define SCRIPT_CALLBACK_FUNC(name) \
		v8::Handle<v8::Value> name(v8::Arguments const& args)
	
	#define SCRIPT_CALLBACK(template, name, callback) \
		template->Set(name, v8::FunctionTemplate::New(callback))

} }; // namespace fieldkit::script

#include "fieldkit/script/Type.h"
#include "fieldkit/script/InstanceHandle.h"
#include "fieldkit/script/Accessor.h"
#include "fieldkit/script/Property.h"


// --- Proxy Class -------------------------------------------------------------
namespace fieldkit { namespace script {

	
	// --- Proxy ---------------------------------------------------------------
	template<class T>
	class ProxyClass {
	public:	
		
		//! returns the Single instance for the template class
		static ProxyClass<T>* Instance() { return &instance; }
		
		ProxyClass<T>* init(char const* name);
		
		//! Expose class property member to JavaScript
		template<class TProperty>
		void expose(TProperty T::*property, const char* propertyName, bool getter, bool setter);
		
		//! Expose class property member to JavaScript whit a custon accessor
		template<class TProperty>
		void expose(TProperty T::*property, const char* propertyName,
					AccessorProperty* accessor, bool getter, bool setter);
		
		//! Registers the class represented by this Proxy within the given context
		void exposeClass(Handle<ObjectTemplate> context);
		
		Handle<Value> exposeObject(T* t, bool withDestroy);
		
		void exposeObject(Handle<Object> context, T* t, 
						  const char* name, bool withDestroy);
		
		T* unwrapObject(Local<Object> holder);
		T* unwrapObject(Local<Value> value);
		
		Handle<FunctionTemplate> getFunctionTemplate() { return functionTemplate; }
		
	private:
		//! Single ProxyClass<T>
		ProxyClass() {};
		~ProxyClass();	

		//! static single class ProxyClass<T>
		static ProxyClass<T> instance;
		
		//! Single JavaScript name
		Local<String> className;
		
		//! V8 FunctionTemplate
		Handle<FunctionTemplate> functionTemplate;
		
		//! V8 ObjectTemplate
		Handle<ObjectTemplate> objectTemplate;
		
		//! Keeps track of accessors to delete them later when not needed anylonger
		std::list<Accessor*> accessors;
	};
	
	// Create singleton instance
	template<class T>
	ProxyClass<T> ProxyClass<T>::instance;
	
	//! JS callback
	template<class T>
	Handle<Value> Create(const Arguments& args);
	
	//! JS callback
	template<class T>
	void Destroy(Persistent<Object> self, void* parameter);
	
	
} }; // namespace fieldkit::script



// --- Implementation ----------------------------------------------------------
namespace fieldkit { namespace script {

	template<class T>
	ProxyClass<T>::~ProxyClass()
	{
		//LOG_INFO("ProxyClass<T>::~ProxyClass - dtor");
		// TODO probably leaks
		/*BOOST_FOREACH(Accessor* accessor, accessors) {
			delete accessor;
			accessor = NULL;
		}*/
		accessors.clear();
	}
	
	template<class T>
	ProxyClass<T>* ProxyClass<T>::init(char const* name)
	{
//		if (!className.IsEmpty()) {
//			v8::String::Utf8Value currentName(className);
//			//If the name is the same just return the instance
//			if (strcmp(*currentName,name)) {
//				//Utils::ReportApiFailure("cProxyV8::ProxyClass", "Error initializing - Class already exist");  
//				LOG_WARN("Error initializing - Class already exist");
//				return NULL;
//			}
//			
//		} else {
			// Always recreate the templates in case the handleScope was left
			className = String::New(name);
			
			functionTemplate = FunctionTemplate::New(Create<T>);
			functionTemplate->SetClassName(className);
			
			// Instance template from which T objects are based on
			objectTemplate = functionTemplate->InstanceTemplate();
			objectTemplate->SetInternalFieldCount(1); // create an internal field for the C++ object
//		}
		
		return Instance();
	}


	// js T constructor function, called when `new T(...)' in js
	template<class T>
	Handle<Value> Create(const v8::Arguments& args)
	{
		// throw if called without `new'
		if (!args.IsConstructCall()) 
			return v8::ThrowException(v8::String::New("Cannot call constructor as function"));
		
		// throw if we didn't get 0 args
		if (args.Length() != 0)
			return v8::ThrowException(v8::String::New("Expected no arguments"));
		
		// create the C++ Handle<T> to be wrapped
		InstanceHandle<T>* t = new InstanceHandle<T>(true);
		
		// make a persistent handle for the instance and make it
		// weak so we get a callback when it is garbage collected
		Persistent<Object> self = Persistent<Object>::New(args.Holder());
		
		// TODO fix this
		//self.MakeWeak(t, WeakReferenceCallback(Destroy<T>));
		
		// set internal field to the C++ point 
		self->SetInternalField(0, External::New(t));
		
		return self;
	}	
	
	/**
	 * Called when the T object is being garbage collected
	 * delete the C++ object and ClearWeak on the Persistent handle.
	 */
	template<class T>
	void Destroy(Persistent<Object> self, void* parameter)
	{
		delete static_cast<T*>(parameter);
		self.ClearWeak();
	}
	
	
	template<class T>
	template<class TProperty>
	void ProxyClass<T>::expose(TProperty T::*property, const char* propertyName,
							   bool getter, bool setter)
	{
		ObjectProperty<T,TProperty>* accessor = new ObjectProperty<T,TProperty>(property);
		accessors.push_back(accessor);
		expose(property, propertyName, accessor, getter, setter);
	}

	template<class T>
	template<class TProperty>
	void ProxyClass<T>::expose(TProperty T::*property, const char* propertyName,
							   AccessorProperty* accessor, bool getter, bool setter)
	{
//		ASSERT(propertyName);
//		ASSERT(setter || getter);
		
		AccessorGetter getterAccessor = getter ? Accessor_GetProperty : 0;
		AccessorSetter setterAccessor = setter ? Accessor_SetProperty : 0;
		Handle<Value> data = External::New(accessor);
		
		objectTemplate->SetAccessor(String::New(propertyName), getterAccessor, setterAccessor, data);
	}
	
	template<class T>
	void ProxyClass<T>::exposeClass(Handle<ObjectTemplate> context)
	{
		context->Set(className, getFunctionTemplate());	
	}
	
	template<class T>
	Handle<Value> ProxyClass<T>::exposeObject(T* t, bool withDestroy)
	{
		// Handle scope for temporary handles.
		HandleScope handleScope;
		
		//Handle<ObjectTemplate> templ = objectTemplate;
		Handle<Object> object = objectTemplate->NewInstance();
		
		// Wrap the raw C++ pointer in an External so it can be referenced
		// from within JavaScript.
		Local<External> externalVal = Local<External>::Cast(object->GetInternalField(0));
		
		InstanceHandle<T>* handle = static_cast<InstanceHandle<T>*>(externalVal->Value());
		handle->set(t, withDestroy);
		
		// Return the result through the current handle scope.  Since each
		// of these handles will go away when the handle scope is deleted
		// we need to call Close to let one, the result, escape into the
		// outer handle scope.
		return handleScope.Close(object);
	}
	
	template<class T>
	void ProxyClass<T>::exposeObject(Handle<Object> context, T* t, 
									 const char* name, bool withDestroy)
	{  
		//HandleScope handleScope;
		context->Set(String::New(name), exposeObject(t, withDestroy));
	}
	
	template<class T>
	T* ProxyClass<T>::unwrapObject(Local<Object> holder)
	{
		Local<External> external = Local<External>::Cast(holder->GetInternalField(0));
		InstanceHandle<T>* handle = InstanceHandle<T>::Cast(external->Value());
		return *(*handle);
	}
	
	template<class T>
	T* ProxyClass<T>::unwrapObject(Local<Value> value)
	{
		Local<External> external = Local<External>::Cast(value);
		InstanceHandle<T>* handle = static_cast<InstanceHandle<T>*>(external->Value());
		return *(*handle);
	}
	
} }; // namespace fieldkit::script

