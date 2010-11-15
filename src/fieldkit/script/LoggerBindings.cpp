/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#include "fieldkit/script/LoggerBindings.h"

using namespace fieldkit::script;

void LoggerBindings::prepare(ObjectTemplatePtr global) 
{
	global->Set(ToString("print"), v8::FunctionTemplate::New(LoggerBindings::print));
	global->Set(ToString("info"), v8::FunctionTemplate::New(LoggerBindings::info));
	global->Set(ToString("warn"), v8::FunctionTemplate::New(LoggerBindings::warn));
	global->Set(ToString("error"), v8::FunctionTemplate::New(LoggerBindings::error));
	
//	defineGlobalFunction("print", LoggerBindings::print);
//	defineGlobalFunction("info", LoggerBindings::info);
//	defineGlobalFunction("warn", LoggerBindings::warn);
//	defineGlobalFunction("error", LoggerBindings::error);
}


// Prints its arguments on stdout separated by spaces and ending with a newline.
ValuePtr LoggerBindings::print(Arguments const& args) 
{
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		HandleScope handle_scope;
		if (first) {
			first = false;
		} else {
			printf(" ");
		}
		v8::String::Utf8Value str(args[i]);
		const char* cstr = ToCString(str);
		printf("%s", cstr);
	}
	printf("\n");
	fflush(stdout);
	return v8::Undefined();
}

ValuePtr LoggerBindings::info(Arguments const& args) 
{	
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		HandleScope handle_scope;
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_INFO(ss.str());	
	return v8::Undefined();
}

ValuePtr LoggerBindings::warn(Arguments const& args) 
{	
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		HandleScope handle_scope;
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_WARN(ss.str());	
	return v8::Undefined();
}

ValuePtr LoggerBindings::error(Arguments const& args) 
{	
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		HandleScope handle_scope;
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_ERROR(ss.str());	
	return v8::Undefined();
}
