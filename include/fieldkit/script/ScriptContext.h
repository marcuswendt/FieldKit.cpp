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
#include <vector>

namespace fieldkit { namespace script {
	
	using v8::Handle;
	
    class Binding;
    
	//
	// Script class; A context using several bindings to execute a javascript file
	//

	//! Allows to run JavaScript files within a set number of C++ to JS bindings using Google's V8 engine
	class ScriptContext {
	public:
		ScriptContext() {};
		~ScriptContext();
		
		//! adds another c++/js script binding
		void add(Binding* binding);
		
		//! removes all script bindings
		void reset();

        //! runs the given script and returns true on success
        bool execute(std::string source);
        
		//! runs the given script file and returns true on success
		bool executeFile(std::string file);
		
        Persistent<Context> getContext() { return context; }
        
        //! creates a new javascript object instance
        Handle<Object> newInstance(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);

        //! calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        //! calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
        
	protected:
		std::vector<Binding*> bindings;
        Persistent<Context> context;
        
		bool executeString(Handle<String> source);
		
		void reportException(TryCatch* handler);
	};
	
} } // fieldkit::script