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
	
	
	//
	// Script class; A context using several bindings to execute a javascript file
	//

	// JavaScript execution class based on Google's V8 engine
	class ScriptContext {
	public:
		ScriptContext() {};
		~ScriptContext();
		
		// adds another script binding
		void add(Binding* binding);
		
		// clears all script bindings
		void clear();

		// executes the given script
		void run(std::string file);
		
	protected:
		std::vector<Binding*> bindings;
		
		StringPtr readFile(std::string path);
		
		bool executeString(StringPtr source, ValuePtr name, 
						   bool print_result, bool report_exceptions);
		
		void reportException(v8::TryCatch* handler);
	};
	
} } // fieldkit::scripting