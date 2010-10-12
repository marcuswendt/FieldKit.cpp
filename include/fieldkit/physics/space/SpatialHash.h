/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/07/2010.
 */

#pragma once

#include "fieldkit/physics/space/Spatial.h"
#include "fieldkit/physics/space/Space.h"

namespace fieldkit { namespace physics {

	//! A 2D spatial hashing class used to efficiently retrieve large numbers of 
	//! uniformly distributed Spatials.
	class SpatialHash : public Space {
	public:
		SpatialHash();
		SpatialHash(Vec3f offset, Vec3f dimension, float cellSize=5.0f);
		~SpatialHash();
		
		void init(Vec3f offset, Vec3f dimension, float cellSize=5.0f);
		
		//! make sure the space can hold a certain amount of spatials
		void reserve(int count) {}

		//! Empties the entire space contents.
		void clear();
		
		//! Adds a new spatial into the hash.
		void insert(Spatial* spatial);
		
		//! Selects all spatials within the given bounding volume.
		void select(BoundingVolume* volume, SpatialListPtr result);
		
	protected:
		vector< vector<Spatial*> > cells;
		int cellsX;
		int cellsY;
		float cellSize;
		
		inline int hash(float position) {
			return (int)(position / cellSize);
		};
	};
	
} } // namespace fieldkit::physics