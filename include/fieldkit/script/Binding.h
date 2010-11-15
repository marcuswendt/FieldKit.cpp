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
	
	// Base class for c++ to v8 binding adapters
	class Binding {
	public:
		Binding() {};
		~Binding() {};	
		
		// called before a script context is created
		virtual void prepare(ObjectTemplatePtr global) {};
		
		// called after the context is created and before the script is run
		virtual void init(ContextPtr context) {};
	};

} } // fieldkit::script