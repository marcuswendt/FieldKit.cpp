/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include <v8.h>
#include "fieldkit/FieldKit.h"

namespace fieldkit { namespace script {
	
	//
	// Typedefs
	//
	// basic types
	typedef v8::String String;
	typedef v8::Handle<v8::String> StringPtr;
	typedef v8::Local<v8::String> StringLocal;
	
	typedef v8::Integer Integer;
	
	typedef v8::Value Value;
	typedef v8::Handle<v8::Value> ValuePtr;
	
	typedef v8::ObjectTemplate ObjectTemplate;
	typedef v8::Handle<v8::ObjectTemplate> ObjectTemplatePtr;
	
	typedef v8::FunctionTemplate FunctionTemplate;
	typedef v8::Handle<v8::FunctionTemplate> FunctionTemplatePtr;
	
	typedef v8::Handle<v8::Object> ObjectPtr;
	typedef v8::Local<v8::Object> ObjectLocal;
	
	// binding tools
	typedef v8::Context Context;
	typedef v8::Handle<v8::Context> ContextPtr;
	
	typedef v8::HandleScope HandleScope;
	
	typedef v8::AccessorInfo AccessorInfo;
	
	typedef v8::Arguments Arguments;
	
	
	//
	// Static Functions
	//
	const char* ToCString(const v8::String::Utf8Value& value);
	
	const std::string ToStdString(v8::Handle<v8::String> handle);
	
	//! Converts a regular C-String to a v8::String
	#define ToString(value) String::New(value)

	
	//
	// Script class; A context using several bindings to execute a javascript file
	//
	
	// Forward declaration for script binding definitions
	class Binding;

	// JavaScript execution class based on Google's V8 engine
	class Script {
	public:
		Script () {};
		~Script();
		
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