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
#include <ctime>

namespace fieldkit { namespace script {
	
	using v8::Handle;
	
    class Module;
    
	//
	// Script class; A context using several bindings to execute a javascript file
	//

	//! Allows to run JavaScript files within a set number of C++ to JS bindings using Google's V8 engine
	class ScriptContext {
	public:
		ScriptContext() {};
		~ScriptContext();
		
		//! adds another c++/js script binding
		void add(Module* module);

        //! runs the given script and returns true on success
        bool execute(std::string sourceOrFile = "");
        
		//! returns true when any of the .js files in the previously defined parent path have been modified
        bool filesModified();
        
        Persistent<Context> getContext() { return context; }
        
        //! creates a new javascript object instance
        Handle<Object> newInstance(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);

        //! calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        //! calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
        
	private:
        //! main script file and parent directory to look for includes
        std::string filePath, parentPath;
        
        //! remember the most recent write time of the newest file
        std::time_t newestWriteTime;

		std::vector<Module*> modules;
        
        //! the javascript execution context
        Persistent<Context> context;
        
        //! executes the given javascript source code
		bool executeString(Handle<String> source);
        
        //! converts a v8 exception into a standard c++ exception
		void reportException(TryCatch* handler);
	};
	
} } // fieldkit::script