/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#include "fieldkit/script/LoggerBinding.h"
#include "fieldkit/Logger.h"

using namespace fieldkit::script;

// Prints its arguments on stdout separated by spaces and ending with a newline.
v8::Handle<Value> LogPrint(Arguments const& args) 
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
        LOG_OUT(ToCString(str));
	}
    LOG_INST->flush();
	return v8::Undefined();
}

v8::Handle<Value> LogInfo(Arguments const& args) 
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

v8::Handle<Value> LogWarn(Arguments const& args) 
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

v8::Handle<Value> LogError(Arguments const& args) 
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


void LoggerBinding::prepare(v8::Handle<ObjectTemplate> global) 
{
	global->Set(ToV8String("print"), FunctionTemplate::New(LogPrint));
	global->Set(ToV8String("info"), FunctionTemplate::New(LogInfo));
	global->Set(ToV8String("warn"), FunctionTemplate::New(LogWarn));
	global->Set(ToV8String("error"), FunctionTemplate::New(LogError));
}

