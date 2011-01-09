/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/ScriptKit.h"

namespace fieldkit { namespace script {
	
	using v8::Handle;
	
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
		
		//! clears all script bindings
		void clear();

		//! runs the given script and returns true on success
		bool execute(std::string file);
		
	protected:
		std::vector<Binding*> bindings;
		
		Handle<String> readFile(std::string path);
		
		bool executeString(Handle<String> source, Handle<Value> name, 
						   bool print_result, bool report_exceptions);
		
		void reportException(TryCatch* handler);
	};
	
} } // fieldkit::script