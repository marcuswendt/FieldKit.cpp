/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/ScriptKit.h"

namespace fieldkit { namespace script {
	
	// Provides bindings for the fieldkit::Logger functions 
	// LOG_INFO, LOG_WARN, LOG_ERROR 
	class LoggerBindings : public Binding {
		
		void prepare(Handle<ObjectTemplate> global);
		
		static Handle<Value> print(Arguments const& args);		
		static Handle<Value> info(Arguments const& args);
		static Handle<Value> warn(Arguments const& args);
		static Handle<Value> error(Arguments const& args);
	};
	
} } // namespace fieldkit::script