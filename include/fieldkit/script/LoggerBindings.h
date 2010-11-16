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
		
		void prepare(HObjectTemplate global);
		
		static HValue print(Arguments const& args);		
		static HValue info(Arguments const& args);
		static HValue warn(Arguments const& args);
		static HValue error(Arguments const& args);
	};
	
} } // fieldkit::scripting