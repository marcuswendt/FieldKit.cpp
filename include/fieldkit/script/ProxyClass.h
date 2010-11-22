///*                                                                           
// *      _____  __  _____  __     ____                                   
// *     / ___/ / / /____/ / /    /    \   FieldKit
// *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
// *   /_/        /____/ /____/ /_____/    http://www.field.io           
// *   
// *	 Created by Marcus Wendt on 15/11/2010.
// */
//
//#pragma once
//
//#include "fieldkit/script/ScriptKit.h"
//
//namespace fieldkit { namespace script {
//
//	template<class T>
//	class ProxyClass {
//	public:
//		//! Singleton ProxyClass<T>
//		ProxyClass() {};
//		~ProxyClass() {};	
//		
//		ProxyClass<T>* init(const char* name);
//		
//		//! returns the singleton instance for the template class
//		//static ProxyClass<T>* getInstance() { return &ProxyClass<T>::_instance; }
//
//	private:
//		
//		//! Singleton JavaScript name
//		LString className;
//		
//		//! static single class ProxyClass<T>
//		//static ProxyClass<T> _instance;
//		
//		//! V8 FunctionTemplate
//		HFunctionTemplate functionTemplate;
//		
//		//! V8 ObjectTemplate
//		HObjectTemplate objectTemplate;
//	};
//	
//	template<class T>
//	HValue Create(const Arguments& args);
//	
//} }; // namespace fieldkit::script
