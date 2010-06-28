/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/06/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {
	
	//! Sets the particles properties to some default values with some variance
	class Initializer : public Behaviour {
	public:
		Initializer();
		~Initializer() {};
		
		void apply(ParticlePtr p);
		
		// Accessors
		void setState(int value) { state = value; }
		int getState() { return state; }
		
		void setLifeTime(float value) { lifeTime = value; }
		float getLifeTime() { return lifeTime; }

		void setLifeTimeVariance(float value) { lifeTimeVariance = value; }
		float getLifeTimeVariance() { return lifeTimeVariance; }
		
		void setSize(float value) { size = value; }
		float getSize() { return size; }

		void setSizeVariance(float value) { sizeVariance = value; }
		float getSizeVariance() { return sizeVariance; }
		
		void setWeight(float value) { weight = value; }
		float getWeight() { return weight; }
		
		void setWeightVariance(float value) { weightVariance = value; }
		float getWeightVariance() { return weightVariance; }
		
		void setDrag(float value) { drag = value; }
		float getDrag() { return drag; }
		
		void setDragVariance(float value) { dragVariance = value; }
		float getDragVariance() { return dragVariance; }
		
		void setForce(Vec3f value) { force = value; }
		Vec3f getForce() { return force; }
		
		void setForceVariance(Vec3f value) { forceVariance = value; }
		Vec3f getForceVariance() { return forceVariance; }
		
		void setLock(bool value) { lock = value; }
		Vec3f getLock() { return lock; }
		
		void setLockVariance(float value) { lockVariance = value; }
		float getLockVariance() { return lockVariance; }
		
	protected:
		int state;
		
		float lifeTime;
		float lifeTimeVariance;
		
		float size;
		float sizeVariance;
		
		float weight;
		float weightVariance;
		
		float drag;
		float dragVariance;
		
		Vec3f force;
		Vec3f forceVariance;
		
		bool lock;
		float lockVariance;
	};
	
} } // namespace fieldkit::physics