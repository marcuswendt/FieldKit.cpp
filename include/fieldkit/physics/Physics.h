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

#include "fieldkit/physics/Behavioural.h"
#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Emitter.h"
#include "fieldkit/physics/Space.h"
#include "Spring.h"

#include <list>
using std::list;

namespace fk { namespace physics {
	class Physics : public Behavioural {
	public:
		Emitter* emitter;
		Space* space;
		
		Physics(Space* space);
		virtual ~Physics();
			
		virtual void update(float dt);
	
	// -- Particles --------------------------------------------------------
	list<Particle*>	particles;
		
	Particle* createParticle();	
	int numParticles() { return particles.size(); }
		
	// -- Springs ----------------------------------------------------------
		list<Spring*>	springs;
		
		int numSprings() { return springs.size(); }

		void addSpring(Spring* spring);
		void removeSpring(Spring* spring);
		
protected:
	list<Particle*>	deadParticles;
		
	virtual void updateParticles(float dt);
	virtual void updateSprings();
	virtual void updateNeighbours();
};

} 
} // namespace fk::physics