/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Emitter.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "Behavioural.h"

namespace fk { namespace physics {
	
	class Physics;
	
	class Emitter : public Behavioural {
	public:
		Physics* physics;
		
		Vec3f position;
		
		int rate;
		int max;
		float interval;
		float time;
		bool isEnabled;
		
		Emitter(Physics* physics);
		~Emitter() {};
		
		virtual void update(float dt);
		virtual Particle* emit(Vec3f location);
		
	protected:
		void applyBehaviours(Particle* p);
		void applyConstraints(Particle* p);
	};
	
} } // namespace fk::physics