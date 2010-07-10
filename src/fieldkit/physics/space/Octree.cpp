/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#include "fieldkit/physics/space/Octree.h"

using namespace fieldkit::physics;

Octree::Octree(Vec3f offset, Vec3f dimension, float minSize)
{ 	
	this->parent = parent;
	this->offset = offset;
	this->minSize = minSize;
	
	// init AABB
	Vec3f halfSize = dimension * 0.5f;
	this->position = offset + halfSize;
	this->extent = halfSize;
	updateBounds();
	
	depth = 0;
	children = new OctreePtr[8];
}

Octree::~Octree() 
{
	data.clear();
	delete[] children;
}

void Octree::clear() 
{
	// clear all inserted spatials
	data.clear();
	
	// clear all children
	for(int i=0; i<8; i++) {
		if(children[i])
			children[i].reset();
	}
}

void Octree::insert(Spatial* s) 
{
	Vec3f p = s->getPosition();

	// check if point is inside box
	if(!this->contains(p)) return;
	
	// only add data leaves when extent is smaller than minsize
	if(extent.x <= minSize || extent.y <= minSize || extent.z <= minSize) {
		data.push_back(s);
		
	} else {
		int octant = getOctantID(p.x - offset.x, p.y - offset.y, p.z - offset.z);

		// create new octree at given octant
		if(children[octant] == 0) {
			Vec3f o = offset;
			if((octant & 1) != 0) o.x += extent.x;
			if((octant & 2) != 0) o.y += extent.y;
			if((octant & 4) != 0) o.z += extent.z;				
			children[octant] = OctreePtr(new Octree(o, extent, minSize));
		}
		
		// insert spatial into child node
		children[octant]->insert(s);
	}

}

void Octree::select(BoundingVolumePtr volume, SpatialList result)
{
	result.clear();
	selectImpl(volume, result);
}

void Octree::selectImpl(BoundingVolumePtr volume, SpatialList result)
{
	// check wether bounding volume and this node intersect at all
	if(!intersects(volume)) return;
	
	// if this node contains spatial data then check which spatials lie within the bounding volume
	if(data.size() > 0) {
		BOOST_FOREACH(SpatialPtr s, data) {
			if(volume->contains(s->getPosition())) {
				result.push_back(s);	
			}
		}

	// otherwise traverse child nodes
	} else if(children) {
		for(int i=0; i<8; i++) {
			if(children[i])
				children[i]->selectImpl(volume, result);
		}
	}
}

int Octree::getOctantID(float x, float y, float z) 
{
	int id = 0;
	if(x >= extent.x) id += 1;
	if(y >= extent.y) id += 2;
	if(z >= extent.z) id += 4;
	return id;
}


OctreePtr Octree::getChild(int i) { 
	return children[i]; 
}

int Octree::getNumChildren() {
	return 8;
}