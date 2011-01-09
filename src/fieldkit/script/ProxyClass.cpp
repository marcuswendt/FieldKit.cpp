/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#include "fieldkit/script/ProxyClass.h"
#include <string.h>

#include "fieldkit/script/ScriptKit.h"

namespace fieldkit { namespace script {
		
	void Accessor_SetProperty(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
	{
		// LOG_INFO("Accessor_SetProperty "<< ToStdString(property));
		
		v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
		AccessorProperty* callback = static_cast<AccessorProperty*>(external->Value());
		callback->set(property, value, info);
	}
	
	v8::Handle<v8::Value> Accessor_GetProperty(v8::Local<v8::String> property, const v8::AccessorInfo& info)
	{
		// LOG_INFO("Accessor_GetProperty "<< ToStdString(property));
		
		v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
		AccessorProperty* callback = static_cast<AccessorProperty*>(external->Value());
		return callback->get(property, info);
	}
	
} }; // namespace fieldkit::script
