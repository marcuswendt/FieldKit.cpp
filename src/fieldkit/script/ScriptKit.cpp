/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 18/11/2010.
 */

#include "fieldkit/script/ScriptKit.h"

namespace fieldkit { namespace script {
	
	const Local<String> ToV8String(const char* value)
	{
		return String::New(value);
	}

	const Local<String> ToV8String(std::string value)
	{
		return ToV8String(value.c_str());
	}
	
	// Extracts a C string from a V8 Utf8Value.
	const char* ToCString(const String::Utf8Value& value) 
	{
		return *value ? *value : "<string conversion failed>";
	}
	
	// Converts a v8::String to a std::string
	const std::string ToStdString(Handle<String> handle)
	{
		return std::string( *String::AsciiValue(handle) );
	}
		
} } // fieldkit::script
