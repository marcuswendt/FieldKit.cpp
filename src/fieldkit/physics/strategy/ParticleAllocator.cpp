/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/08/2010.
 */

#include "fieldkit/physics/strategy/ParticleAllocator.h"

using namespace fieldkit::physics;

void ParticleAllocator::apply(Physics* physics) 
{
	physics->addParticle( new Particle() );
}