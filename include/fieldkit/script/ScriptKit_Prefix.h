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
    using std::string;
    
    
    //
    // Macros
    //
#define SET_METHOD(obj, name, callback) \
    obj->Set(v8::String::NewSymbol(name), v8::FunctionTemplate::New(callback)->GetFunction())
    
#define SET_PROTOTYPE_METHOD(templ, name, callback) \
do { \
    v8::Local<v8::Signature> __callback##_SIG = v8::Signature::New(templ); \
    v8::Local<v8::FunctionTemplate> __callback##_TEM = \
    v8::FunctionTemplate::New(callback, v8::Handle<v8::Value>(), \
    __callback##_SIG); \
    templ->PrototypeTemplate()->Set(v8::String::NewSymbol(name), \
    __callback##_TEM); \
} while (0)
    
    
	//
	// Static Functions
	//
	
	//! Converts a regular C-String to a v8::String
	const Local<String> ToV8String(const char* value);
	
	const Local<String> ToV8String(std::string value);
	
	const char* ToCString(const String::Utf8Value& value);
	
	const std::string ToStdString(Handle<String> handle);
	
} } // namespace fieldkit::script
