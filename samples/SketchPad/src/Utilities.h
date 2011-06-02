/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - SketchPad
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 02/05/2011.

 */
#pragma once

#include "fieldkit/script/Module.h"

namespace field {
    
    //! Provides a random javascript object with various functions to generate random numbers
    class Utilities : public fieldkit::script::Module {
        void Initialize(v8::Handle<v8::Object> global);
    };
    
}