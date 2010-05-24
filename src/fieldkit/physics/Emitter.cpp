/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Emitter.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#include "Emitter.h"
#include "Physics.h"

namespace fk { namespace physics {

	Emitter::Emitter(Physics* physics) {
		this->physics = physics;
		
		rate = 1;
		interval = 1000.0;
		max = 100;		
	}
	
	Emitter::~Emitter() {
	}
	
	void Emitter::update(float dt) {
		if(!isEnabled) return;
		
		// check if its time to start emitting
		time += dt;
		if(time < interval) return;
		
		time = 0;
		
		// emit particles
		int i=0;
		//while(i < rate && physics->numParticles() < max) {
		while(i < rate && physics->numParticles() < max) {
			emit(position);
			i += 1;
		}
	}
	
	
	void Emitter::emit(Vec3f location) {
		
	}
	
} } // namespace fk::physics