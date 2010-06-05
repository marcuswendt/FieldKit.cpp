/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 05/06/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Constraint.h"

namespace fk { namespace physics {
	
	//! Constrains a particle's movement by locking one of its axes.
	class PlaneConstraint : public Constraint {
	public:
		enum Axis { AXIS_X = 0, AXIS_Y = 1, AXIS_Z = 2};
		
		Axis axis;
		float constraint;
		
		PlaneConstraint() : 
			axis(AXIS_Y), constraint(0.0f) {}
		
		PlaneConstraint(Axis axis, float constraint) {
			this->axis = axis;
			this->constraint = constraint;
		}
		
		void apply(ParticlePtr p);
	};
} } // namespace fk::physics