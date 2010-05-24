/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Particle.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "Particle.h"

namespace fk { namespace physics {
	
	Particle::Particle() {}
	
	Particle::~Particle() {}
	
	void Particle::update(float dt) {
		this->age += dt;
	}
	
} } // fk::physics

