/*
 *  ScriptKit.cpp
 *  FieldKit
 *
 *  Created by Marcus Wendt on 18/11/2010.
 *  Copyright 2010 Marcus Wendt. All rights reserved.
 *
 */

#include "fieldkit/script/ScriptKit.h"

namespace fieldkit { namespace script {
	
	const LString ToV8String(const char* value)
	{
		return String::New(value);
	}

	const LString ToV8String(std::string value)
	{
		return ToV8String(value.c_str());
	}
	
	// Extracts a C string from a V8 Utf8Value.
	const char* ToCString(const String::Utf8Value& value) 
	{
		return *value ? *value : "<string conversion failed>";
	}
	
	// Converts a v8::String to a std::string
	const std::string ToStdString(HString handle)
	{
		return std::string( *String::AsciiValue(handle) );
	}
		
} } // fieldkit::script
