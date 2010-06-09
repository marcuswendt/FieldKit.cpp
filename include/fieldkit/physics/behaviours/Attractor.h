/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Behaviour.h"

namespace fk { namespace physics {
	
	class AttractorPoint : public WeightedBehaviour {
	public:
		AttractorPoint(Space* space) : WeightedBehaviour(space) {
			setRange(0.1f);
		};
		
		void prepare(float dt);
		void apply(ParticlePtr p);
		
		void setPosition(Vec3f location) { position.set(location); }
		Vec3f getPosition() { return position; }
		
		void setRange(float value) { range = value; }
		float getRange() { return range; }
		
		float getRangeAbs() { return rangeAbs; }
		
	protected:
		Vec3f position;
		float range;
		
		float rangeAbs;
		float rangeAbsSq;
	};
	
} } // namespace fk::physics