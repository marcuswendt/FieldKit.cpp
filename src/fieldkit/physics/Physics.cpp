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

Physics::Physics(SpacePtr space) 
{
	this->space = space;
	
	numParticles = 0;

	setParticleUpdate(PhysicsStrategyPtr(new ParticleUpdate()));
	setSpringUpdate(PhysicsStrategyPtr(new SpringUpdate()));
	setNeighbourUpdate(PhysicsStrategyPtr(new EmptyStrategy()));
}

Physics::~Physics() 
{
	particles.clear();
	springs.clear();
	
	particleUpdate.reset();
	springUpdate.reset();
	neighbourUpdate.reset();
}

void Physics::update(float dt)
{
	if(emitter)
		emitter->update(dt);
	
	particleUpdate->apply(this);
	springUpdate->apply(this);
	neighbourUpdate->apply(this);
}

// -- Particles ------------------------------------------------------------
// check if we still have a dead particle that we can reuse, otherwise create a new one
ParticlePtr Physics::createParticle() 
{
	numParticles++;
	//for(ParticlePtr pIt = particles.begin(); pIt != particles.end();) {
	BOOST_FOREACH(ParticlePtr p, particles) {
		if(!p->isAlive) return p;
	}
	
	logger() << "WARNING: running out of particles" << std::endl;
	// FIXME
	return particles[0];
}

// allocates a bunch of new particles
void Physics::allocParticles(int count) 
{
	particles.reserve(count);
	for(int i=0; i<count; i++){
		particles.push_back( allocParticle() );
	}
	
	space->reserve(count);
}

// allocates a single particle, override this method to create custom Particle types
ParticlePtr Physics::allocParticle()
{
	return new Particle();
}


// -- Springs --------------------------------------------------------------
void Physics::addSpring(SpringPtr spring) 
{
	springs.push_back(spring);
}

void Physics::removeSpring(SpringPtr spring)
{
	// TODO
//	springs.erase(spring);
}

