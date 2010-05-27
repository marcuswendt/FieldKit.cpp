/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Particle.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "fieldkit/physics/Spatial.h"
#include "fieldkit/physics/Behavioural.h"
#include "fieldkit/math/BoundingVolume.h"
#include "fieldkit/FieldKit.h"
#include <list>
using std::list;

namespace fk { namespace physics {

class Particle : public Spatial
{
	public:
		list<void *> neighbours;
		BoundingVolume *neighbourBound;
		
		int state;
		float age;
		float lifeTime;
		float size;
		float drag;
		bool isLocked;
		bool isAlive;
		
		Vec3f position;
		Vec3f prev;
		Vec3f force;
		
		Particle() {};
		~Particle() {};
		
		// lifecycle
		virtual void init(Vec3f location);
		virtual void update(float dt);
		
		virtual void updateState(float dt);
		virtual void updatePosition();
		Vec3f *getSpatialPosition(){ return &position; };

		// verlet integration
		void lock();
		void unlock();
		void clearVelocity();
		void scaleVelocity(float s);
};
	
} } // namespace fk::physics