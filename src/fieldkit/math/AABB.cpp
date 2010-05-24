/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   AABB.cpp
 *	 Created by Marcus Wendt on 24/05/2010.
 */

#include "AABB.h"

namespace fk { namespace math {
	
	void AABB::setWidth(float value) {
		extent.x = value / 2.0;
	}
	
	float AABB::getWidth() {
		return extent.x * 2.0;
	}

	void AABB::setHeight(float value) {
		extent.y = value / 2.0;
	}
	
	float AABB::getHeight() {
		return extent.y * 2.0;
	}

	void AABB::setDepth(float value) {
		extent.z = value / 2.0;
	}
	
	float AABB::getDepth() {
		return extent.z * 2.0;
	}
} } // namespace fk::math