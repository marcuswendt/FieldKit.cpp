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

// -----------------------------------------------------------------------------

#include <v8.h>

namespace fieldkit { namespace script {
	
	//
	// Typedefs - mostly typing savers
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
	
} } // namespace fieldkit::script

// -----------------------------------------------------------------------------

// Core classes
#include "fieldkit/script/Binding.h"
#include "fieldkit/script/ScriptContext.h"

// Bindings
#include "fieldkit/script/LoggerBindings.h"