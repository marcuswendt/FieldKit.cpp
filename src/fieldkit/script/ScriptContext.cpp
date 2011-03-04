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

using namespace fieldkit::script;


// Reads a file into a v8 string.
Handle<String> ReadScriptFile(std::string path) 
{
	FILE* file = fopen(path.c_str(), "rb");
	if (file == NULL) return Handle<String>();
	
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
	Handle<String> result = String::New(chars, size);
	delete[] chars;
	return result;
}



// -- Script Context ----------------------------------------------------------
ScriptContext::~ScriptContext()
{
	reset();
    
    LOG_INFO("ScriptContext::~ScriptContext - V8 dispose");
	v8::V8::Dispose();
}

void ScriptContext::add(Binding* binding)
{
	bindings.push_back(binding);
}

void ScriptContext::reset()
{
	BOOST_FOREACH(Binding* b, bindings) {
		delete b;
	}
	bindings.clear();
}

bool ScriptContext::execute(std::string _file) 
{
	// make sure v8 is still active
	if(v8::V8::IsDead())
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
	Handle<String> fileName = String::New(_file.c_str());
	Handle<String> source = ReadScriptFile(_file.c_str());
	if (source.IsEmpty()) {
        throw "Error reading '"+ _file +"'";
		return false;
	}
	
	if (!executeString(source, fileName, false, true)) {
        throw "Error executing script";
		return false;
	}

	// Deinitialise bindings
	BOOST_FOREACH(Binding* b, bindings) {
		b->deinit(context);
	}
	
	return true;
}


// -- Helpers ------------------------------------------------------------------

// Executes a string within the current v8 context.
bool ScriptContext::executeString(Handle<String> source, Handle<Value> name,
						   bool print_result, bool report_exceptions) 
{
	HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
	if (script.IsEmpty()) {
		// Print errors that happened during compilation.
		if (report_exceptions)
			reportException(&try_catch);
		return false;
	} else {
		Handle<Value> result = script->Run();
		if (result.IsEmpty()) {
			// Print errors that happened during execution.
			if (report_exceptions)
				reportException(&try_catch);
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

void ScriptContext::reportException(TryCatch* try_catch)
{
	HandleScope handle_scope;
	String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = ToCString(exception);
	v8::Handle<v8::Message> message = try_catch->Message();
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

		String::Utf8Value stack_trace(try_catch->StackTrace());
		if (stack_trace.length() > 0) {
			const char* stack_trace_string = ToCString(stack_trace);
			ss << stack_trace_string;
		}

		throw ss.str();
	}
}
