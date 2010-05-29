/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Weighable.h
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "fieldkit/Fieldkit.h"

namespace fk { namespace physics {
	
	class Weighable {
	public:
		Weighable() { weight = 1.0; }
		void setWeight(float value) { this->weight = value; }
		float getWeight() { return this->weight; }
					   
	protected:
		float weight;
	};
	
} } // namespace fk::physics