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

	//! typedefs
	class Spatial;
	//typedef shared_ptr<Spatial> SpatialPtr;
	typedef Spatial* SpatialPtr;
	typedef vector<SpatialPtr> SpatialList;
	typedef SpatialList* SpatialListPtr;
	
	//! interface for all types of elements that can be put into a space
	class Spatial {
	public:
		Spatial() {};
		~Spatial() {};

		virtual Vec3f getPosition() = 0;
	};
	
	//! basic spatial implementation that wraps a Vec3f into a spatial
	class PointSpatial : public Spatial {
	public:
		PointSpatial(Vec3f p = Vec3f::zero()) : position(p) {};
		~PointSpatial() {};
		
		Vec3f getPosition() { return position; };
		
	protected:
		Vec3f position;
	};
	
} } // namespace fieldkit::physics