/*
 *  BoundingVolume.cpp
 *  FieldKit
 *
 *  Created by Marcus Wendt on 10/07/2010.
 *  Copyright 2010 FIELD. All rights reserved.
 *
 */

#include "fieldkit/math/BoundingVolume.h"

#include "fieldkit/math/AABB.h"
#include "fieldkit/math/SphereBound.h"

using namespace fieldkit::math;

bool BoundingVolume::intersects(BoundingVolumePtr volume) {

	switch (type) {
		// AABB interesections
		case BOUNDING_BOX: {
			AABB* a = (AABB*)this;
			
			switch (volume->type) {
				// AABB x AABB
				case BOUNDING_BOX:
					return intersects(a, (AABB*)volume.get() );
					
				// AABB x Sphere
				case BOUNDING_SPHERE:
					return intersects(a, (SphereBound*)volume.get() );
			};
			break;
		}
			
		// Sphere intersections
		case BOUNDING_SPHERE: {
			AABB* a = (AABB*)this;
			
			switch (volume->type) {
				// Sphere x Sphere
				case BOUNDING_SPHERE:
					return intersects(a, (SphereBound*)volume.get() );
					
				// AABB x Sphere
				case BOUNDING_BOX:
					return intersects( (AABB*)volume.get(), a );
			};
			break;
		}
	}

	logger() << "Warning: Unsupported intersection type!" << endl;
	return false;
}


// -- Intersection Helpers -----------------------------------------------------
bool BoundingVolume::intersects(AABB* a, AABB* b)
{
	Vec3f tmp = a->position - b->position;
	
	return	abs(tmp.x) <= (a->extent.x + b->extent.x) &&
	abs(tmp.y) <= (a->extent.y + b->extent.y) &&
	abs(tmp.z) <= (a->extent.z + b->extent.z);
}

bool BoundingVolume::intersects(SphereBound* a, SphereBound* b)
{
	Vec3f delta = a->position - b->position;
	float d = delta.length();
	float r1 = a->radius;
	float r2 = b->radius;
	return d <= r1 + r2 && d >= abs(r1 - r2);
}

bool BoundingVolume::intersects(AABB* a, SphereBound* b)
{
	Vec3f min = a->min;
	Vec3f max = a->max;
	
	Vec3f center = b->position;
	float radius = b->radius;
	
	float s = 0;
	float d = 0;
	
	// find the square of the distance
	// from the sphere to the box
	if(center.x < min.x) {
		s = center.x - min.x;
		d = s * s;
		
	} else if (center.x > max.x) {
		s = center.x - max.x;
		d += s * s;
	}
	
	if (center.y < min.y) {
		s = center.y - min.y;
		d += s * s;
		
	} else if (center.y > max.y) {
		s = center.y - max.y;
		d += s * s;
	}
	
	if (center.z < min.z) {
		s = center.z - min.z;
		d += s * s;
	} else if (center.z > max.z) {
		s = center.z - max.z;
		d += s * s;
	}
	
	return d <= radius * radius;
}