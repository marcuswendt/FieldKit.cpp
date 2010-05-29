/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Particle.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Particle.h"

namespace fk { namespace physics {
	
	void Particle::init(Vec3f location) {
		set(location);
		clearVelocity();
		
		// set defaults
		state = 0;
		age = 0; 
		lifeTime = 1000;
		isAlive = true;
		
		size = 1.0;
		isLocked = false;
		
		setWeight(1.0);
		
		drag = 0.03;
		force = Vec3f(0,0,0);
	}
	
	void Particle::update(float dt) {
		updateState(dt);
		updatePosition();
	}
	
	// update lifecycle
	void Particle::updateState(float dt) {
		age += dt;
		if(age > lifeTime)
			isAlive = false;	
	}

	// -- verlet integration ---------------------------------------------------
	void Particle::updatePosition() {
		if(isLocked) return;
		
		tmp.set(*this);
		
		x += (x - prev.x) + force.x;
		y += (y - prev.y) + force.y;
		z += (z - prev.z) + force.z;
		
//		printf("pos: %f %f %f prev %f %f %f force %f %f %f\n", 
//			   x, y, z, 
//			   prev.x, prev.y, prev.z, 
//			   force.x, force.y, force.z);
		
		prev.set(tmp);
		
		scaleVelocity(1.0 - drag);
		force = force.zero();
	}
	
	void Particle::lock() {
		isLocked = true;
	}
	
	void Particle::unlock() {
		isLocked = false;
	}
	
	void Particle::clearVelocity() {
		prev.set(*this);
	}
	
	void Particle::scaleVelocity(float s) {
		prev = prev.lerp(s, *this);
	}
	
	// -- getters & setters ----------------------------------------------------
	void Particle::setWeight(float value) {
		this->weight = value;
		this->invWeight = 1.0 / value;
	}
} } // fk::physics

