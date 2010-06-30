/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/05/2010.
 */

#include "fieldkit/physics/space/Space.h"

using namespace fieldkit::physics;

Vec3f Space::getCenter() {
	return this->position;
}

float Space::toAbsolute(float value) {
	return value * getWidth();
}

float Space::toRelative(float value) {
	return value / getWidth();
}
