/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Physics.h"

using namespace fieldkit::physics;

Physics::Physics(Space* space) 
{
	this->space = space;
	
	numParticles = 0;
	numAllocated = 0;
	
	emitter = NULL;
	ownsSpace = true;
	
	particleAllocator = NULL;
	particleUpdate = NULL;
	springUpdate = NULL;
	neighbourUpdate = NULL;

	setParticleAllocator(new ParticleAllocator());
	setParticleUpdate(new ParticleUpdate());
	setSpringUpdate(new SpringUpdate());
	//setNeighbourUpdate(new EmptyStrategy());
}

Physics::~Physics() 
{
	// strategies
	if(particleAllocator != NULL) {
		delete particleAllocator;
		particleAllocator = NULL;
	}

	if(particleUpdate != NULL) {
		delete particleUpdate;
		particleUpdate = NULL;
	}

	if(springUpdate != NULL) {
		delete springUpdate;
		springUpdate = NULL;
	}

	if(neighbourUpdate != NULL) {
		delete neighbourUpdate;
		neighbourUpdate = NULL;
	}

	// springs & particles
	destroySprings();
	destroyParticles();

	// emitter
	if(emitter != NULL) {
		delete emitter;
		emitter = NULL;
	}
	
	// space
	if(ownsSpace) {
		delete space;
		space = NULL;
	}
}

void Physics::update(float dt)
{
	if(emitter != NULL)
		emitter->update(dt);
	
	if(particleUpdate != NULL)
		particleUpdate->apply(this);

	if(springUpdate = NULL)
		springUpdate->apply(this);

	if(neighbourUpdate != NULL)
		neighbourUpdate->apply(this);
}

// -- Particles ----------------------------------------------------------------
// check if we still have a dead particle that we can reuse, otherwise create a new one
Particle* Physics::createParticle() 
{
	numParticles++;
	BOOST_FOREACH(Particle* p, particles) {
		if(!p->isAlive) return p;
	}
	
	if(particleAllocator != NULL)
		particleAllocator->apply(this);

	return particles.back();
}

// allocates a bunch of new particles
void Physics::allocParticles(int count) 
{
	numAllocated = particles.size() + count;
	space->reserve(numAllocated);
	particles.reserve(numAllocated);
	
	for(int i=0; i<count; i++)
		particleAllocator->apply(this);
}

void Physics::addParticle(Particle* particle)
{
	particles.push_back(particle);
}

void Physics::destroyParticles()
{
	BOOST_FOREACH(Particle* p, particles) {
		delete p;
		p = NULL;
	}
	particles.clear();
}


// -- Springs ------------------------------------------------------------------
void Physics::addSpring(Spring* spring) 
{
	springs.push_back(spring);
}

void Physics::removeSpring(Spring* spring)
{
	// TODO
//	springs.erase(spring);
}

void Physics::destroySprings()
{
	BOOST_FOREACH(Spring* s, springs) {
		delete s;
		s = NULL;
	}
	springs.clear();
}

// -- Setters -----------------------------------------------------------------
void Physics::setParticleAllocator( PhysicsStrategy* strategy )
{
	if(particleAllocator != NULL) {
		delete particleAllocator;
		particleAllocator = NULL;
	}
	particleAllocator = strategy;
}

void Physics::setParticleUpdate( PhysicsStrategy* strategy )
{
	if(particleUpdate != NULL) {
		delete particleUpdate;
		particleUpdate = NULL;
	}
	particleUpdate = strategy;
}

void Physics::setSpringUpdate( PhysicsStrategy* strategy )
{
	if(springUpdate != NULL) {
		delete springUpdate;
		springUpdate = NULL;
	}
	springUpdate = strategy;
}

void Physics::setNeighbourUpdate( PhysicsStrategy* strategy )
{
	if(neighbourUpdate != NULL) {
		delete neighbourUpdate;
		neighbourUpdate = NULL;
	}
	neighbourUpdate = strategy;
}
