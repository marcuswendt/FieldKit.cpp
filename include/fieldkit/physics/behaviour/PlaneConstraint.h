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
	class WallConstraint : public Constraint {
	public:
		WallConstraint(Axis axis_ = AXIS_Y,
					   bool isPositive_ = true,
					   float height_=0.0f, 
					   float bouncyness_ = 1.0f) : 
			axis(axis_), isPositive(isPositive_),
			bouncyness(bouncyness_), height(height_) {}
		
		~WallConstraint() {}
		
		void apply(ParticlePtr p);

		// Accessors
		void setAxis(Axis value) { axis = value; }
		Axis getAxis() { return axis; };
		
		void setIsPositive(bool value) { isPositive = value; }
		bool getIsPositive() { return isPositive; };
		
		void setHeight(float value) { height = value; }
		float getHeight() { return height; };
		
		void setBouncyness(float value) { bouncyness = value; }
		float getBouncyness() { return bouncyness; };

	protected:
		Axis axis;
		bool isPositive;
		float height;
		float bouncyness;
	};
	
} } // namespace fieldkit::physics