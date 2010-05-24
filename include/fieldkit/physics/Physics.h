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

#include "Behavioural.h"
#include "Particle.h"
#include "Emitter.h"
#include "Space.h"

#include <list>
using std::list;

namespace fk { namespace physics {
	class Physics : public Behavioural {
	public:
		Emitter* emitter;
		Space* space;
		
		Physics();
		virtual ~Physics();
			
		virtual void update(float dt);
	
		// -- Particles --------------------------------------------------------
		list<Particle*>	particles;
		
		void addParticle(Particle* p);	
		void removeParticle(Particle* p);
		
		int numParticles() { return particles.size(); }
		
		// -- Springs ----------------------------------------------------------
		//list<Particle>	particles;
		
	protected:
		virtual void updateParticles(float dt);
		virtual void updateSprings();
		virtual void updateNeighbours();
	};

} } // namespace fk::physics