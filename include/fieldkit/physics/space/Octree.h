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

	//! Implements a spatial subdivision tree to work efficiently with large numbers
	//! of 3D spatials (particles or points). 
	//! This octree can only be used for particle type objects and does NOT 
	//! support 3D mesh geometry as other forms of Octrees do.
	class Octree : public Space {
	public:
		
		// -- Octree Nodes -----------------------------------------------------
		class Node : public AABB {
		public:
			enum NodeType {
				TYPE_LEAF, TYPE_BRANCH
			};
			
			NodeType type;
			Vec3f offset;
			int depth;
			float minSize;
			bool isEmpty;
			
			Node() { isEmpty = true; };
			virtual ~Node() {};
			
			virtual void init(Vec3f const& offset, Vec3f const& dimension, float minSize=5.0f, int depth=0);			
			virtual void destroy(bool ownsSpatials);
			virtual void clear() {};
			virtual void insert(Spatial* spatial) {};
			virtual void select(BoundingVolume* volume, SpatialListPtr result) {};
			
			// Acessors
			void setOffset(Vec3f offset);
			Vec3f getOffset() { return offset; };
		};
		
		typedef Node* NodePtr;
		
		class Branch : public Node {
		public:
			vector<NodePtr> children;
			
			Branch();
			~Branch();
			
			void init(Vec3f const& offset, Vec3f const& dimension, float minSize=5.0f, int depth=0);
			void destroy(bool ownsSpatials);
			void clear();
			void insert(Spatial* spatial);
			void select(BoundingVolume* volume, SpatialListPtr result);
			
			//! Computes the local child octant/cube index for the given point.
			inline int getOctantID(float x, float y, float z);
		};
		
		class Leaf : public Node {
		public:	
			SpatialList data;
			
			Leaf();
			~Leaf();

			void destroy(bool ownsSpatials);
			void clear();
			void insert(Spatial* spatial);
			void select(BoundingVolume* volume, SpatialListPtr result);
		};
		
		
		// -- Octree Main ------------------------------------------------------
		
		//! Constructs a new Octree node.
		Octree();
		Octree(Vec3f offset, Vec3f dimension, float minSize=5.0f);
		~Octree();

		//! Initializes the tree.
		void init(Vec3f offset, Vec3f dimension, float minSize=5.0f);
		
		//! make sure the space can hold a certain amount of spatials
		void reserve(int count) {};
		
		//! Empties the entire space contents.
		void clear();
		
		//! Adds a new spatial to the tree structure. All points are stored within leaf nodes only.
		void insert(Spatial* spatial);
		
		//! Selects all spatials within the given bounding volume.
		void select(BoundingVolume* volume, SpatialListPtr result);
		
		// Accessors
		Octree::NodePtr getRoot() { return root; };
		
	protected:
		NodePtr root;
	};

} } // namespace fieldkit::physics