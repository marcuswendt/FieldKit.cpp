/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include "fieldkit/physics/space/Spatial.h"

#include "fieldkit/math/MathKit.h"
using namespace fieldkit::math;

namespace fieldkit { namespace physics {
	
	class Particle : public Spatial {
	public:
		//! Lifetime set to infinite
		const static int LIFETIME_PERPETUAL = -1;

		int state;
		float age;
		float lifeTime;
		
		//! flag set if particles lifeTime is up
		bool isAlive;
		
		//! used by Springs and some Behaviours
		float weight;
		
		//! inverted weight
		float invWeight;
		
		//! wether this particle can be affected by behaviours
		bool isLocked;
		
		Vec3f position;

		//! previous position in the simulation space
		Vec3f prev;
		
		//! amount of friction applied to velocity
		float drag;
		
		//! force accumulator applied to this particle (set to zero after each update)
		Vec3f force;

		//! optional user data
		void *userData;

		Particle();
		~Particle();
		
		// Lifecycle
		virtual void init(Vec3f location);
		virtual void update(float dt);
		
		void updateState(float dt);
		void updatePosition();
		
		// ! manual kill, set alive to false
		void kill();

		// Verlet integration
		void lock();
		void unlock();
		void clearVelocity();
		void scaleVelocity(float s);
		
		// Accessors
		SpatialType getType() { return TYPE_PARTICLE; };
		
		Vec3f getPosition() { return position; };
		void setPosition(Vec3f location) { this->position = location; };
		
		virtual void setSize(float radius);
		float getSize();
		
		void setWeight(float value);
		inline float getWeight() { return this->weight; };
		inline float getInvWeight() { return this->invWeight; };
		Vec3f getVelocity();
		float getSpeed();
				
		SpatialListPtr getNeighbours() { return neighbours; };
		
	protected:
		//! physical size in the simulation space
		float size;

		//! list of neighbours this particle is aware of
		SpatialListPtr neighbours;
	};
	
	// Define 
	typedef Particle* ParticlePtr;
} } // namespace fieldkit::physics