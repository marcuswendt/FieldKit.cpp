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
	
	using namespace v8;
	using v8::Handle;
	
	//
	// Static Functions
	//
	
	//! Converts a regular C-String to a v8::String
	const Local<String> ToV8String(const char* value);
	
	const Local<String> ToV8String(std::string value);
	
	const char* ToCString(const String::Utf8Value& value);
	
	const std::string ToStdString(Handle<String> handle);
	
} } // namespace fieldkit::script

// Core classes
#include "fieldkit/script/Binding.h"
#include "fieldkit/script/ScriptContext.h"

// Proxy 
#include "fieldkit/script/ProxyClass.h"

// Bindings
#include "fieldkit/script/LoggerBindings.h"
