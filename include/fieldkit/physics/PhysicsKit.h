/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 22/06/2010.
 */
#pragma once

//#pragma message("PhysicsKit is included")

// Dependencies
#include "fieldkit/FieldKit.h"

#include "fieldkit/math/MathKit.h"
using namespace fieldkit::math;

// space
#include "fieldkit/physics/space/Spatial.h"
#include "fieldkit/physics/space/Space.h"
#include "fieldkit/physics/space/BasicSpace.h"
#include "fieldkit/physics/space/Octree.h"

// physics
#include "fieldkit/physics/Behaviour.h"
#include "fieldkit/physics/Constraint.h"
#include "fieldkit/physics/Behavioural.h"

#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Emitter.h"
#include "fieldkit/physics/Spring.h"
#include "fieldkit/physics/Physics.h"

// behaviours
#include "fieldkit/physics/behaviour/Attractor.h"
#include "fieldkit/physics/behaviour/Boundary.h"
#include "fieldkit/physics/behaviour/Flocking.h"
#include "fieldkit/physics/behaviour/Force.h"
#include "fieldkit/physics/behaviour/Initializer.h"
#include "fieldkit/physics/behaviour/PlaneConstraint.h"
#include "fieldkit/physics/behaviour/Random.h"
#include "fieldkit/physics/behaviour/SphereConstraint.h"