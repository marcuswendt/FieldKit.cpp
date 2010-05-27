/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   
 *	 Created by David Hoe on 26/05/2010.
 */

#pragma once

#include "cinder/Vector.h"
#include "fieldkit/physics/Behaviour.h"
#include "fieldkit/physics/Particle.h"

namespace fk { 
namespace physics {

class FlockBehaviour : public Behaviour
{
public:
	float weight;
	FlockBehaviour();
	virtual void apply(Particle* p);

};

}
} // namespace fk::physics