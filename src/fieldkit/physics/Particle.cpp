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
	
	void Particle::init(Vec3f location) {
		this->position = location;
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
		force = Vec3f();
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
		
		Vec3f tmp = position;

		//force *= _timestepSq;
		
		position.x += (position.x - prev.x) + force.x;
		position.y += (position.y - prev.y) + force.y;
		position.z += (position.z - prev.z) + force.z;
		
		prev = tmp;
		
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
		prev = position;
	}
	
	void Particle::scaleVelocity(float s) {
		prev = prev.lerp(s, position);
	}
	
	// -- getters & setters ----------------------------------------------------
	void Particle::setWeight(float value) {
		this->weight = value;
		this->invWeight = 1.0 / value;
	}
} } // fk::physics

