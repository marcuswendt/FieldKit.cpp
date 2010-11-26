/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {
	
	//! Drags the particle towards a point in space
	class AttractorPoint : public WeightedBehaviour {
	public:
		AttractorPoint(Space* space) : WeightedBehaviour(space) {
			setRange(0.1f);
			position = Vec3f::zero();
		};
		
		void apply(Particle* p);
		
		void setPosition(Vec3f location) { position.set(location); }
		Vec3f getPosition() { return position; }
		
		void setRange(float value);
		float getRange() { return range; }
		
		float getRangeAbs() { return rangeAbs; }
		
	protected:
		Vec3f position;
		float range;
		
		float rangeAbs;
		float rangeAbsSq;
	};
	
} } // namespace fieldkit::physics