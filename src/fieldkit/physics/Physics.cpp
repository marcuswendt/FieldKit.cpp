/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Physics.h"

#include "fieldkit/physics/strategy/ParticleAllocator.h"
#include "fieldkit/physics/strategy/ParticleUpdate.h"
#include "fieldkit/physics/strategy/SpringUpdate.h"
#include "fieldkit/physics/strategy/NeighbourUpdate.h"

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

//	setParticleAllocator(new ParticleAllocator());
//	setParticleUpdate(new ParticleUpdate());
//	setSpringUpdate(new SpringUpdate());
	// no neighbour update strategy by default
	
	particleAllocator = new ParticleAllocator();
	particleUpdate = new ParticleUpdate();
	springUpdate = new SpringUpdate();
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
		particleUpdate->apply(this, dt);

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
	for (vector<Particle*>::iterator pit = particles.begin(); pit != particles.end(); pit++) {
		if(!(*pit)->isAlive) return *pit;
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
	if(springs.size() == 0) return;
	
	BOOST_FOREACH(Spring* s, springs) {
		delete s;
	}
	springs.clear();
}

// -- Setters -----------------------------------------------------------------
void Physics::setParticleAllocator(ParticleAllocator* strategy )
{
	if(particleAllocator != NULL) {
		delete particleAllocator;
	}
	particleAllocator = strategy;
}

void Physics::setParticleUpdate(ParticleUpdate* strategy )
{
	if(particleUpdate != NULL) {
		delete particleUpdate;
	}
	particleUpdate = strategy;
}

void Physics::setSpringUpdate(SpringUpdate* strategy )
{
	if(springUpdate != NULL) {
		delete springUpdate;
	}
	springUpdate = strategy;
}

void Physics::setNeighbourUpdate(NeighbourUpdate* strategy )
{
	if(neighbourUpdate != NULL) {
		delete neighbourUpdate;
	}
	neighbourUpdate = strategy;
}
