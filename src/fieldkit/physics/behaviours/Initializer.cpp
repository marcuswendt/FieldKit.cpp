/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 27/06/2010.
 */

#include "fieldkit/physics/behaviours/Initializer.h"

using namespace fieldkit::physics;

Initializer::Initializer() {
	state = 0;
	
	lifeTime = 1000.0f;
	lifeTimeVariance = 0.0f;
	
	size = 1.0;
	sizeVariance = 0.0f;
	
	weight = 1.0;
	weightVariance = 0.0f;
	
	drag = 0.03f;
	dragVariance = 0.0f;
	
	force = Vec3f();
	forceVariance = Vec3f();
	
	lock = false;
	lockVariance = 0.0f;
}

void Initializer::apply(ParticlePtr p) {
}