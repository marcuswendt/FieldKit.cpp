///*                                                                           
// *      _____  __  _____  __     ____                                   
// *     / ___/ / / /____/ / /    /    \   FieldKit
// *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
// *   /_/        /____/ /____/ /_____/    http://www.field.io           
// *   
// *	 Created by Marcus Wendt on 15/11/2010.
// */
//
//#include "fieldkit/script/ProxyClass.h"
//
//using namespace fieldkit::script;
//
//template<class T>
//ProxyClass<T>* ProxyClass<T>::init(const char* name)
//{
//	if (!className.IsEmpty()) {
//		String::Utf8Value currentName(className);
//		//If the name is the same just return the instance
//		if (strcmp(*currentName,name)) {
//			//Utils::ReportApiFailure("cProxyV8::ProxyClass", "Error initializing - Class already exist");  
//			LOG_WARN("Error initializing - Class already exist");
//			return NULL;
//		}
//		
//	} else {
//		className = String::New(name);
//		
//		functionTemplate = FunctionTemplate::New(Create<T>);
//		functionTemplate->SetClassName(className);
//		
//		// Instance template from which T objects are based on
//		objectTemplate = functionTemplate->InstanceTemplate();
//		objectTemplate->SetInternalFieldCount(1); // create an internal field for the C++ object
//	}
//	
//	//return getInstance();
//	return this;
//}
//
//
//// js T constructor function, called when `new T(...)' in js
//template<class T>
//HValue Create(const Arguments& args)
//{
//	// throw if called without `new'
//	if (!args.IsConstructCall()) 
//		return ThrowException(String::New("Cannot call constructor as function"));
//	
//	// throw if we didn't get 0 args
//	if (args.Length() != 0)
//		return ThrowException(String::New("Expected no arguments"));
//	
//	// create the C++ Handle<T> to be wrapped
//	//InstanceHandle<T>* t = new InstanceHandle<T>(true);
//	T* t = new T();
//	
//	// make a persistent handle for the instance and make it
//	// weak so we get a callback when it is garbage collected
//	PObject self = PObject::New(args.Holder());
//	
//	// TODO
//	//self.MakeWeak(t, WeakReferenceCallback(Class_Destroy<T>));
//	
//	// set internal field to the C++ point 
//	self->SetInternalField(0, External::New(t));
//	
//	return self;
//}