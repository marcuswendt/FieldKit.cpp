/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Physics.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {

	class Space;
	class Emitter;
	class Spring;

	class Physics : public Behavioural {
	public:
		Emitter* emitter;
		Space* space;
		
		Physics(Space* space);
		virtual ~Physics();
			
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
		
		// Accessors
		void setEmptySpaceOnUpdate(bool enabled) { emptySpaceOnUpdate = enabled; };
		bool getEmptySpaceOnUpdate() { return emptySpaceOnUpdate; };

		void setDoUpdateNeighbours(bool enabled) { doUpdateNeighbours = enabled; };
		bool getDoUpdateNeighbours() { return doUpdateNeighbours; };
	protected:	
		int numParticles;

		bool doUpdateNeighbours;
		bool emptySpaceOnUpdate;

		virtual void updateParticles(float dt);
		virtual void updateSprings();
		virtual void updateNeighbours();
		
		virtual ParticlePtr allocParticle();
	};

} } // namespace fieldkit::physics