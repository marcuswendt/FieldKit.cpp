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
	typedef vector<Spatial*> SpatialList;
	typedef SpatialList* SpatialListPtr;
	
	//! interface for all types of elements that can be put into a space
	class Spatial {
	public:
		enum SpatialType {
			TYPE_POINT, TYPE_PARTICLE, TYPE_OTHER
		};
		
		Spatial() {};
		virtual ~Spatial() {};

		virtual int const getType() { return TYPE_OTHER; };
		
		virtual Vec3f const& getPosition() = 0;
		virtual void setPosition(Vec3f const& location) = 0;
	};
	
	//! basic spatial implementation that wraps a Vec3f into a spatial
	class PointSpatial : public Spatial {
	public:
		PointSpatial(Vec3f p = Vec3f::zero()) : position(p) {};
		~PointSpatial() {};

		int const getType() { return TYPE_POINT; };
		
		Vec3f const& getPosition() { return position; };
		void setPosition(Vec3f const& location) { this->position = location; };
		
	protected:
		Vec3f position;
	};
	
} } // namespace fieldkit::physics