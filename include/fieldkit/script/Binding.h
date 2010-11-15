/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/Script.h"

namespace fieldkit { namespace script {
	
	// Forward declations
	class Script;
	
	// Base class for c++ to v8 binding adapters
	class Binding {
	public:
		Binding() {};
		~Binding() {};	
		
		// called before a script context is created
		virtual void prepare(ObjectTemplatePtr global) {};
		
		// called after the context is created and before the script is run
		virtual void init(ContextPtr context) {};
		
	protected:
//		// the current script engine this bind is operating on
//		Script* script;
//		
//		// called each time before a script is executed
//		virtual void apply() = 0;
//		
//		// defines a simple global function
//		void defineGlobalFunction(std::string name, v8::InvocationCallback callback) {
//			script->global->Set(ToJSString(name), v8::FunctionTemplate::New(callback));
//		}
//		
//		void defineGlobalAccessor(std::string name, v8::AccessorGetter getter, v8::AccessorSetter setter=0) {
//			script->global->SetAccessor(ToJSString(name), getter, setter);
//		}
	};

} } // fieldkit::script