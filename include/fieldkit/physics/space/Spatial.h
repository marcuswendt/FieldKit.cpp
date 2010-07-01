/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 24/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"

namespace fieldkit { namespace physics {

	//! interface for all types of elements that can be put into a space
	class Spatial {
	public:
		Spatial() {};
		~Spatial() {};

		virtual Vec3f getSpatialPosition() = 0;
		//virtual BoundingVolume getSpatialBounds() = 0;
	};

} } // namespace fieldkit::physics