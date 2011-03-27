/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/ScriptKit_Prefix.h"

namespace fieldkit { namespace script {
	
	//! Interface for C++ to v8 bindings
	class Module {
	public:
		Module() {};
		~Module() {};
        
		virtual void Initialize(v8::Handle<v8::Object> target) {};
        
        
    protected:
        
        //! Helper method to attach a c++ callback function to a v8 object
        void Attach(v8::Handle<v8::Object> target, const char* name, v8::InvocationCallback callback);

        //! Helper method to attach a v8 function template to a v8 object
        void Attach(v8::Handle<v8::Object> target, const char* name, Handle<FunctionTemplate> functionTemplate);
	};

} } // fieldkit::script