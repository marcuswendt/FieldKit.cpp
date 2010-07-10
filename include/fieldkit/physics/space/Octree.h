/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/physics/space/Spatial.h"
#include "fieldkit/physics/space/Space.h"

namespace fieldkit { namespace physics {

	// typedefs
	class Octree;
	typedef shared_ptr<Octree> OctreePtr;
	typedef OctreePtr* OctreeList;
	
	//! Implements a spatial subdivision tree to work efficiently with large numbers
	//! of 3D spatials (particles or points). 
	//! This octree can only be used for particle type objects and does NOT 
	//! support 3D mesh geometry as other forms of Octrees do.
	class Octree : public Space {
	public:
		//! Constructs a new Octree node.
		Octree(Vec3f offset, Vec3f dimension, float minSize=5.0f);
		~Octree();

		//! Empties the entire space contents.
		void clear();
		
		//! Adds a new spatial to the tree structure. All points are stored within leaf nodes only.
		void insert(SpatialPtr s);
		
		//! Selects all spatials within the given bounding volume.
		void select(BoundingVolumePtr volume, SpatialList result);
		
		// Acessors
		void setOffset(Vec3f offset);
		Vec3f getOffset() { return offset; };
		
		OctreePtr getChild(int i);
		int getNumChildren();
		
	protected:
		OctreePtr parent;
		Vec3f offset;
		int depth;
		float minSize;
		SpatialList data;
		OctreeList children;

		//! Computes the local child octant/cube index for the given point.
		inline int getOctantID(float x, float y, float z);
		
		void selectImpl(BoundingVolumePtr volume, SpatialList result);
	};

} } // namespace fieldkit::physics