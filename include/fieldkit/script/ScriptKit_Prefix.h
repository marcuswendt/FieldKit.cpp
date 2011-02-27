/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 28/06/2010.
 */

#pragma once

#include <v8.h>
#include <string>

namespace fieldkit { namespace script {
	
	using namespace v8;
	using v8::Handle;
    using std::string;
    
	//
	// Static Functions
	//
	
	//! Converts a regular C-String to a v8::String
	const Local<String> ToV8String(const char* value);
	
	const Local<String> ToV8String(std::string value);
	
	const char* ToCString(const String::Utf8Value& value);
	
	const std::string ToStdString(Handle<String> handle);
	
} } // namespace fieldkit::script
