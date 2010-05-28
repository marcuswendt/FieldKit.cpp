/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   AttractorPoint.cpp
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/AttractorPoint.h"

namespace fk { namespace physics {
	
	void AttractorPoint::apply(Particle* p) {
		p->force += (position - p->position).normalized() * weight;
	};
	
} } // namespace fk::physics