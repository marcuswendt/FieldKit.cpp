/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   BoundingVolume.h
 *	 Created by Marcus Wendt on 23/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"

namespace fieldkit { namespace math {

	//! typedefs
	class BoundingVolume;
	typedef shared_ptr<BoundingVolume> BoundingVolumePtr;
	
	//! forward declarations
	class AABB;
	class SphereBound;
	
	//! Base class for all types of bounding volumes
	class BoundingVolume {
	public:
		enum BoundingVolumeType {
			BOUNDING_BOX, BOUNDING_SPHERE
		};
		
		BoundingVolumeType type;
		
		Vec3f position;
		virtual bool contains(Vec3f p) = 0;
		bool intersects(BoundingVolumePtr volume);
		
		// Accessors
		Vec3f getPosition() { return position; };
		
	protected:
		// intersection helpers
		inline bool intersectAABBxAABB(AABB* a, AABB* b);
		inline bool intersectSpherexSphere(SphereBound* a, SphereBound* b);
		inline bool intersectAABBxSphere(AABB* a, SphereBound* b);
	};

} } // namespace fieldkit::math