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
#include <boost/filesystem/operations.hpp>
#include <sstream>
#include <stdexcept>

using namespace fieldkit::script;

// Reads a file into a stl string.
static string LoadFile(string const& path)
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
    
    string contents = string(chars, size);
    delete chars;
    
    return contents;
}

// Parses a given source string for '#include "filename"' macros and replaces these with the actual file contents
static void ResolveIncludes(string const& path, string& source)
{
    string key = "#include";
    
    size_t match = source.find(key);
    if(match == string::npos) 
        return;
    
    size_t start = source.find('"', match);
    size_t end = source.find('"', start+1);
    size_t len = end - start -1;
    
    string file = source.substr(start+1, len);
    string content = LoadFile(path +"/" + file);
    
    if(content == "") 
        content = " print(\"ERROR: Couldnt include file '"+ file +"')";
    
    source.replace(match, end - match +1, content);
    
    ResolveIncludes(path, source);
}


static std::time_t GetNewestFileWriteTime(string const& _path)
{
    namespace fs = boost::filesystem;
    
    std::size_t newestWrite = 0;
    
    // check if any of the script files have changed
    if(!fs::exists(_path)) return newestWrite;
    
    fs::directory_iterator endIt;
    for(fs::directory_iterator it(_path); it != endIt; ++it) {
        std::time_t fileWrite = fs::last_write_time(it->path());
        if(fileWrite > newestWrite) {
            newestWrite = fileWrite;
        }
    }
    return newestWrite;
}


// -- Script Context ----------------------------------------------------------
ScriptContext::~ScriptContext()
{
    filePath = "";
    parentPath = "";
    
    context.Dispose();
    
	BOOST_FOREACH(Binding* b, bindings) {
		delete b;
	}
	bindings.clear();
}

void ScriptContext::add(Binding* binding)
{
	bindings.push_back(binding);
}

bool ScriptContext::execute(std::string sourceOrFile) 
{
    // check if arguments are valid
    if(sourceOrFile == "" && filePath == "") {
        throw std::runtime_error("Calling execute without arguments requires to set a filepath before!");
        return false;
    }
    
    
    // check if argument is a path to a script file
    std::string source = "";
    
    namespace fs = boost::filesystem;
    if(fs::exists(sourceOrFile)) {
        this->filePath = sourceOrFile;
        this->parentPath = boost::filesystem::path(filePath).parent_path().string();
        
    // if not and its not empty - its probably a javascript source string
    } else if(sourceOrFile != "") {
        this->filePath = "";
        this->parentPath = "";
        source = sourceOrFile;
    }
    
    
    // when filePath is set - load source from file
    if(filePath != "") {
//        LOG_INFO("loading script from path "<< parentPath);
        
        // load main script file and resolve includes
        source = LoadFile(filePath);
        ResolveIncludes(parentPath, source);
        
        // remember newest write time to see if any file has changed later
        newestWriteTime = GetNewestFileWriteTime(parentPath);
        
        if(source == "") {
            throw std::runtime_error("Error reading '"+ filePath +"'");
            return false;
        }
    }
    
    
    // make sure v8 is still active
	if(V8::IsDead())
		return false;
	
	HandleScope handleScope;
    
	// -- Create Context --
	
	// Create a template for the global object.
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	
    
    // Attach bindings
	BOOST_FOREACH(Binding* b, bindings) {
		b->attach(global);
	}
    
    // Clean up old execution environment if set
    context.Dispose();
    
	// Create a new execution environment containing the attached functions
    context = Context::New(NULL, global);
    
	// Enter the newly created execution environment.
	Context::Scope contextScope(context);
	
	// -- Execute Script --
    Handle<String> _source = String::New(source.c_str());

	if(!executeString(_source)) {
        //throw std::runtime_error("Error executing script");
		return false;
	}
    
	return true;
}

bool ScriptContext::filesModified()
{
    // no parent path set yet
    if(parentPath == "") return false;
    
    std::time_t writeTime = GetNewestFileWriteTime(parentPath);
    if(writeTime > newestWriteTime) {
        LOG_INFO("Script file modification detected");
        newestWriteTime = writeTime;
        return true;
        
    } else {
        return false;
    }
}

Handle<Object> ScriptContext::newInstance(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv) 
{
    HandleScope handleScope;
    TryCatch tryCatch;
    
    Handle<Value> value = localContext->Get(name);
    Handle<Function> func = Handle<Function>::Cast(value);
    Handle<Value> result = func->NewInstance(argc, argv);
    
    if(result.IsEmpty())
        reportException(&tryCatch);
    
    return handleScope.Close(Handle<Object>::Cast(result));
}

Handle<Value> ScriptContext::call(Handle<Object> localContext, const char* name, int argc, Handle<Value>* argv)
{
    return call(localContext, String::New(name), argc, argv);
}

Handle<Value> ScriptContext::call(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv) 
{
    HandleScope handleScope;
    TryCatch tryCatch;
    
    Handle<Value> value = localContext->Get(name);
    
    // check if function exists within given context
    if(!value->IsUndefined()) {
        Handle<Function> func = Handle<Function>::Cast(value);
        Handle<Value> result = func->Call(localContext, argc, argv);
        
        if(result.IsEmpty())
            reportException(&tryCatch);
        
        return handleScope.Close(result);
    }
    
    return Undefined();
}


// -- Helpers ------------------------------------------------------------------

// Executes a string within the current v8 context.
bool ScriptContext::executeString(Handle<String> source) 
{
//	HandleScope handleScope;
	TryCatch tryCatch;
    
	Handle<Script> script = Script::Compile(source);
    
	if (script.IsEmpty()) {
        reportException(&tryCatch);
		return false;
        
	} else {
		Handle<Value> result = script->Run();
		if (result.IsEmpty()) {
            reportException(&tryCatch);
			return false;
            
		} else {
			if(!result->IsUndefined()) {
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
//	HandleScope handleScope;
    
	String::Utf8Value exception(tryCatch->Exception());
	const char* exceptionString = ToCString(exception);
	Handle<Message> message = tryCatch->Message();
    
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		throw  std::runtime_error("Unknown error "+ std::string(exceptionString));

	} else {
		// Print (filename):(line number): (message).
		String::Utf8Value filename(message->GetScriptResourceName());
		const char* filenameString = ToCString(filename);
		int linenum = message->GetLineNumber();

        std::stringstream ss;
        
        // Print filename and linenum if applicable
        if(!message->GetScriptResourceName()->IsUndefined()) {
            ss << filenameString <<":"<< linenum <<" ";
        }
        
		ss << exceptionString << std::endl;

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
        
        throw std::runtime_error(ss.str());
	}
}
