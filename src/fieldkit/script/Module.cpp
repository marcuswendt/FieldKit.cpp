/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/03/2011.
 */

#include "fieldkit/script/Module.h"

namespace fieldkit { namespace script {
    
    void Module::Attach(v8::Handle<v8::Object> target, const char* name, v8::InvocationCallback callback)
    {
        target->Set(String::New(name), FunctionTemplate::New(callback)->GetFunction());        
    }
    
    void Module::Attach(v8::Handle<v8::Object> target, const char* name, Handle<FunctionTemplate> templ)
    {
        target->Set(String::New(name), templ->GetFunction());
    }
    
} }
