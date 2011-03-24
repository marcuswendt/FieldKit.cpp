/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#pragma once

#include "fieldkit/script/Binding.h"

namespace fieldkit { namespace script {
	
	//! Provides bindings for the fieldkit::Logger functions 
	// e.g. LOG_INFO, LOG_WARN, LOG_ERROR 
	class Logger : public Binding {
		void attach(Handle<ObjectTemplate> global);
	};
	
} }