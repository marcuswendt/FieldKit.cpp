/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/03/2011.
 */

#pragma once

#include "fieldkit/script/Module.h"

namespace fieldkit { namespace script {
	
    //! Provides a random javascript object with various functions to generate random numbers
	class Random : public Module {
		void Initialize(Handle<Object> global);
	};
	
} }