/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include <vector>
#include "fieldkit/physics/PhysicsKit_Prefix.h"
#include "fieldkit/physics/Behavioural.h"

namespace fieldkit { namespace physics {

	// FWD
    class Space;
	class Emitter;
    class Spring;
	class ParticleAllocator;
	class SpringUpdate;
	class NeighbourUpdate;
	class ParticleUpdate;

	//! base class for all types of physics systems
	class Physics : public Behavioural {
	public:
		Emitter* emitter;
		Space* space;

		//! number of currently active particles
		int numParticles;
		
		// Constructors
		Physics(Space* space);
		virtual ~Physics();
			
		virtual void update(float dt);
	
		// Particles
        std::vector<Particle*> particles;
		
		void allocParticles(int count);
		int getNumAllocated() { return numAllocated; }
		Particle* createParticle();
		void addParticle(Particle* particle);
		int getNumParticles() { return numParticles; }
		void destroyParticles();

		// Springs
        std::vector<Spring*> springs;

		void addSpring(Spring* spring);
		void removeSpring(Spring* spring);
		int getNumSprings() { return springs.size(); }
		void destroySprings();

		// Strategies
		void setParticleAllocator(ParticleAllocator* strategy);
		ParticleAllocator* getParticleAllocator() { return particleAllocator; };

		void setParticleUpdate(ParticleUpdate* strategy);
		ParticleUpdate* getParticleUpdate() { return particleUpdate; };

		void setSpringUpdate(SpringUpdate* strategy);
		SpringUpdate* getSpringUpdate() { return springUpdate; };

		void setNeighbourUpdate(NeighbourUpdate* strategy);
		NeighbourUpdate* getNeighbourUpdate() { return neighbourUpdate; };

		// Accessors
		void setOwnsSpace(bool isOwner) { ownsSpace = isOwner; }
		bool getOwnsSpace() { return ownsSpace; }
	
	protected:
		bool ownsSpace;
		int numAllocated;

		ParticleAllocator* particleAllocator;
		ParticleUpdate* particleUpdate;
		SpringUpdate* springUpdate;
		NeighbourUpdate* neighbourUpdate;
	};

} } // namespace fieldkit::physics