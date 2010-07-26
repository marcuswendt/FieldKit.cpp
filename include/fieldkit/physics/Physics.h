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

	//! base class for all types of physics systems
	class Physics : public Behavioural {
	public:
		EmitterPtr emitter;
		SpacePtr space;

		//! number of currently active particles
		int numParticles;
		
		//! time since last update
		float dt;
		
		// ---------------------------------------------------------------------
		
		// Constructors
		Physics(SpacePtr space);
		~Physics();
			
		virtual void update(float dt);
	
		// Particles
		vector<ParticlePtr> particles;
			
		ParticlePtr createParticle();	
		void allocParticles(int count);
			
		int getNumParticles() { return numParticles; }
			
		// Springs
		vector<SpringPtr> springs;
		
		int numSprings() { return springs.size(); }

		void addSpring(SpringPtr spring);
		void removeSpring(SpringPtr spring);
		
		// ---------------------------------------------------------------------

		// Accessors
		void setParticleUpdate(PhysicsStrategyPtr strategy) { particleUpdate = strategy; };
		PhysicsStrategyPtr getParticleUpdate() { return particleUpdate; };

		void setSpringUpdate(PhysicsStrategyPtr strategy) { springUpdate = strategy; };
		PhysicsStrategyPtr getSpringUpdate() { return springUpdate; };

		void setNeighbourUpdate(PhysicsStrategyPtr strategy) { neighbourUpdate = strategy; };
		PhysicsStrategyPtr getNeighbourUpdate() { return neighbourUpdate; };
		
	protected:	
		PhysicsStrategyPtr particleUpdate;
		PhysicsStrategyPtr springUpdate;
		PhysicsStrategyPtr neighbourUpdate;
		
		virtual ParticlePtr allocParticle();
	};

} } // namespace fieldkit::physics