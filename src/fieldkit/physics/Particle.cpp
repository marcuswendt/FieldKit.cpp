/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Particle.h"

using namespace fieldkit::physics;

Particle::Particle() : 
	isAlive(false), isLocked(false),
	state(0), age(0.0f), lifeTime(1000.0f) {
}

void Particle::init(Vec3f location) {
	position.set(location);
	clearVelocity();

	// set defaults
	state = 0;
	age = 0.0f;
	lifeTime = 1000.0f;
	isAlive = true;
	
	size = 1.0f;
	isLocked = false;
	
	setWeight(1.0);		
	drag = 0.03f;
	force = Vec3f(0,0,0);
}

void Particle::update(float dt) {
	updateState(dt);
	if(isLocked) return;
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
	tmp.set(position);
	
	position.x += (position.x - prev.x) + force.x;
	position.y += (position.y - prev.y) + force.y;
	position.z += (position.z - prev.z) + force.z;
	
	prev.set(tmp);
	
	scaleVelocity(drag);
	force = force.zero();
}

void Particle::lock() {
	isLocked = true;
}

void Particle::unlock() {
	isLocked = false;
}

void Particle::clearVelocity() {
	prev.set(position);
}

void Particle::scaleVelocity(float s) {
	prev = prev.lerp(s, position);
}

// -- Accessors ----------------------------------------------------------------
void Particle::setWeight(float value) {
	this->weight = value;
	this->invWeight = 1.0f / value;
}


Vec3f fk::physics::Particle::getVelocity()
{
	return prev - position;
}

float fk::physics::Particle::getSpeed()
{
	return getVelocity().length();
}
