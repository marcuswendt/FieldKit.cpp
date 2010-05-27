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

#include "fieldkit/physics/behaviours/FlockBehaviour.h"
#include "fieldkit/Fieldkit.h"

namespace fk { 
namespace physics {

class FlockAttract : public FlockBehaviour
{
public:
	void apply(Particle *Particle);
};
	
}
} // namespace fk::physics