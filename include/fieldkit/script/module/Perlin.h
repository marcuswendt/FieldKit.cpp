/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/05/2011.
 */

#pragma once

#include "fieldkit/script/Module.h"

namespace fieldkit { namespace script {
    
    //! Wraps Cinders Perlin noise class
	class Perlin : public Module {
		void Initialize(v8::Handle<v8::Object> target);
	};
    
} }