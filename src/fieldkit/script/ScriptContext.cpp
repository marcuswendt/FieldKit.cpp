/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#include "fieldkit/script/ScriptContext.h"
#include "fieldkit/script/Binding.h"
#include "fieldkit/Logger.h"
#include <boost/foreach.hpp>
#include <sstream>
#include <stdexcept>

using namespace fieldkit::script;

// Reads a file into a stl string.
std::string ReadScriptFile(std::string path) 
{
	FILE* file = fopen(path.c_str(), "rb");
	if (file == NULL) return "";
	
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);
	
	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = fread(&chars[i], 1, size - i, file);
		i += read;
	}
	fclose(file);
    
    return std::string(chars, size);
}

// -- Script Context ----------------------------------------------------------
ScriptContext::~ScriptContext()
{
	reset();
//    V8::Dispose();
}

void ScriptContext::add(Binding* binding)
{
	bindings.push_back(binding);
}

void ScriptContext::reset()
{
    context.Dispose();
    
	BOOST_FOREACH(Binding* b, bindings) {
		delete b;
	}
	bindings.clear();
}

bool ScriptContext::execute(std::string _source) 
{
    // make sure v8 is still active
	if(V8::IsDead())
		return false;
	
	HandleScope handleScope;
    
	// -- Create Context --
	
	// Create a template for the global object.
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	
	BOOST_FOREACH(Binding* b, bindings) {
		b->prepare(global);
	}
	
	// Create a new execution environment containing the built-in functions
    context = Context::New(NULL, global);
	
	// Enter the newly created execution environment.
	Context::Scope contextScope(context);
	
	// -- Expose Script Objects --
	BOOST_FOREACH(Binding* b, bindings) {
		b->init(context);
	}
	
	// -- Execute Script --
    Handle<String> source = String::New(_source.c_str());

	if(!executeString(source, false, true)) {
        //throw std::runtime_error("Error executing script");
		return false;
	}
    
	// Deinitialise bindings
	BOOST_FOREACH(Binding* b, bindings) {
		b->deinit(context);
	}
	
	return true;

}

bool ScriptContext::executeFile(std::string file) 
{
    std::string source = ReadScriptFile(file);
    if(source == "") {
        throw std::runtime_error("Error reading '"+ file +"'");
        return false;
    }
    return execute(source);
}

Handle<Object> ScriptContext::newInstance(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv) 
{
    HandleScope handleScope;
    
    Handle<Value> value = localContext->Get(name);
    Handle<Function> func = Handle<Function>::Cast(value);
    Handle<Value> result = func->NewInstance(argc, argv);
    
    return handleScope.Close(Handle<Object>::Cast(result));
}

Handle<Value> ScriptContext::call(Handle<Object> localContext, const char* name, int argc, Handle<Value>* argv)
{
    return call(localContext, String::New(name), argc, argv);
}

Handle<Value> ScriptContext::call(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv) {
    HandleScope handleScope;
    Handle<Value> value = localContext->Get(name);
    Handle<Function> func = Handle<Function>::Cast(value);
    Handle<Value> result = func->Call(context->Global(), argc, argv);
    return handleScope.Close(result);
}


// -- Helpers ------------------------------------------------------------------

// Executes a string within the current v8 context.
bool ScriptContext::executeString(Handle<String> source, bool print_result, bool report_exceptions) 
{
	HandleScope handleScope;
	TryCatch tryCatch;
	Handle<Script> script = Script::Compile(source);
	if (script.IsEmpty()) {
		// Print errors that happened during compilation.
		if (report_exceptions)
			reportException(&tryCatch);
		return false;
	} else {
		Handle<Value> result = script->Run();
		if (result.IsEmpty()) {
			// Print errors that happened during execution.
			if (report_exceptions)
				reportException(&tryCatch);
			return false;
		} else {
			if (print_result && !result->IsUndefined()) {
				// If all went well and the result wasn't undefined then print
				// the returned value.
				String::Utf8Value str(result);
				const char* cstr = ToCString(str);
				printf("%s\n", cstr);
			}
			return true;
		}
	}
}

void ScriptContext::reportException(TryCatch* tryCatch)
{
	HandleScope handleScope;
    
	String::Utf8Value exception(tryCatch->Exception());
	const char* exception_string = ToCString(exception);
	Handle<Message> message = tryCatch->Message();
    
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		throw exception_string;

	} else {
		// Print (filename):(line number): (message).
		String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = ToCString(filename);
		int linenum = message->GetLineNumber();

        std::stringstream ss;
		ss << filename_string <<":"<< linenum <<" "<< exception_string << std::endl;

		// Print line of source code.
		String::Utf8Value sourceline(message->GetSourceLine());
		const char* sourceline_string = ToCString(sourceline);

		ss << sourceline_string << std::endl;

		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) {
			ss << " ";
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) {
			ss << "^";
		}
		ss << std::endl;

		String::Utf8Value stack_trace(tryCatch->StackTrace());
		if (stack_trace.length() > 0) {
			const char* stack_trace_string = ToCString(stack_trace);
			ss << stack_trace_string;
		}
        
        LOG_ERROR( ss.str() );
//        throw ss.str();
	}
}
