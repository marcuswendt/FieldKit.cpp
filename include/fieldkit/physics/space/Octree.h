/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/physics/PhysicsKit.h"

namespace fieldkit { namespace physics {

	//! Implements a spatial subdivision tree to work efficiently with large numbers
	//! of 3D spatials (particles or points). 
	//! This octree can only be used for particle type objects and does NOT 
	//! support 3D mesh geometry as other forms of Octrees do.
	class Octree : public Space {
	public:
		//! Constructs an octree root node.
		Octree(Vec3f offset, Vec3f dimension, float minSize=10.0f);
		~Octree();

		//! Empties the entire space contents.
		void clear();
		
		//! Adds a new spatial to the tree structure. All points are stored within leaf nodes only.
		void insert(Spatial* s);
		
		//! Selects all spatials within the given bounding volume.
		void select(BoundingVolume* volume, list<Spatial*> result);
		
		// Acessors
		void setOffset(Vec3f offset);
		Vec3f getOffset() { return offset; };
		
	protected:
		Octree* parent;
		Vec3f offset;
		int depth;
		float minSize;
		vector<Spatial*> data;
		vector<Octree*> children;

		//! Constructs an octree child node.
		Octree(Octree* parent, Vec3f offset, Vec3f halfSize, float minSize);

		//! Initializes this octree node.
		void init(Octree* parent, Vec3f offset, Vec3f halfSize, float minSize);

		//! Computes the local child octant/cube index for the given point.
		inline int getOctantID(float x, float y, float z);

	};

} } // namespace fieldkit::physics