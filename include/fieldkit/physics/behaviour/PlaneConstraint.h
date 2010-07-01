/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 05/06/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {
	
	enum Axis { AXIS_X = 0, AXIS_Y = 1, AXIS_Z = 2};
	
	//! Constrains a particle's movement by locking one of its axes.
	class PlaneConstraint : public Constraint {
	public:
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
	
	//! Makes sure a particle never moves below a certain minimum floor height
	class FloorConstraint : public Constraint {
	public:
		Axis axis;
		float height;

		FloorConstraint() : axis(AXIS_Y), height(0.0f) {
			setBouncyness(1.0f);
		}
		~FloorConstraint() {}
		
		void apply(ParticlePtr p);

		void setBouncyness(float value);
		float getBouncyness() { return bouncyness; };

	protected:
		float bouncyness;
	};
	
} } // namespace fieldkit::physics