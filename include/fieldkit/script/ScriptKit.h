/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 28/06/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"

#include <v8.h>

namespace fieldkit { namespace script {

	// shorthand versions of common v8 types
	#define td(name) \
		typedef v8::name name;
	
	#define tdhlp(name) \
		typedef v8::name name; \
		typedef v8::Handle<name> H##name; \
		typedef v8::Local<name> L##name; \
		typedef v8::Persistent<name> P##name;
	
	tdhlp(String);
	tdhlp(Integer);
	tdhlp(Value);	
	tdhlp(ObjectTemplate);
	tdhlp(FunctionTemplate);
	tdhlp(Object);
	tdhlp(Context);

	td(Arguments);
	td(External);
	td(HandleScope);	
	td(AccessorInfo);
	td(TryCatch);
	
	//
	// Static Functions
	//
	
	//! Converts a regular C-String to a v8::String
	const LString ToV8String(const char* value);
	
	const LString ToV8String(std::string value);
	
	const char* ToCString(const String::Utf8Value& value);
	
	const std::string ToStdString(HString handle);
	
} } // namespace fieldkit::script

// Core classes
#include "fieldkit/script/Binding.h"
#include "fieldkit/script/ScriptContext.h"

// Bindings
#include "fieldkit/script/LoggerBindings.h"
