/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {

	// FWD
	class Emitter;

	//! base class for all types of physics systems
	class Physics : public Behavioural {
	public:
		Emitter* emitter;
		Space* space;

		//! number of currently active particles
		int numParticles;
		
		//! time since last update
		float dt;
		
		// Constructors
		Physics(Space* space);
		virtual ~Physics();
			
		virtual void update(float dt);
	
		// Particles
		vector<Particle*> particles;
		
		void allocParticles(int count);
		int getNumAllocated() { return numAllocated; }
		Particle* createParticle();
		void addParticle(Particle* particle);
		int getNumParticles() { return numParticles; }
		void destroyParticles();

		// Springs
		vector<Spring*> springs;

		void addSpring(Spring* spring);
		void removeSpring(Spring* spring);
		int getNumSprings() { return springs.size(); }
		void destroySprings();

		// Strategies
		void setParticleAllocator(PhysicsStrategy* strategy);
		PhysicsStrategy* getParticleAllocator() { return particleAllocator; };

		void setParticleUpdate(PhysicsStrategy* strategy);
		PhysicsStrategy* getParticleUpdate() { return particleUpdate; };

		void setSpringUpdate(PhysicsStrategy* strategy);
		PhysicsStrategy* getSpringUpdate() { return springUpdate; };

		void setNeighbourUpdate(PhysicsStrategy* strategy);
		PhysicsStrategy* getNeighbourUpdate() { return neighbourUpdate; };

	protected:	
		int numAllocated;

		PhysicsStrategy* particleUpdate;
		PhysicsStrategy* springUpdate;
		PhysicsStrategy* neighbourUpdate;
		PhysicsStrategy* particleAllocator;
	};

} } // namespace fieldkit::physics