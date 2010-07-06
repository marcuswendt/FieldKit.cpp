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
	init(NULL, offset, dimension * 0.5f, minSize);
}

Octree::Octree(Octree* parent, Vec3f offset, Vec3f halfSize, float minSize)
{
	init(parent, offset, halfSize, minSize);	
}

Octree::~Octree() 
{
	for(int i=0; i<8; i++) {
		if(children[i])
			delete children[i];
	}
	delete children;
	children = NULL;
	
//	delete data;
//	data = NULL;
}

void Octree::init(Octree* parent, Vec3f offset, Vec3f halfSize, float minSize)
{
	this->parent = parent;
	this->offset = offset;
	this->minSize = minSize;
	
	// init AABB
	this->position = offset + halfSize;
	this->extent = halfSize;
	updateBounds();
	
	depth = 0;	
	
	children = new Octree*[8];
	clear();
}

void Octree::clear() 
{
	// clear all children
	for(int i=0; i<8; i++) {
		if(children[i]) {
			children[i]->clear();
		}
		children[i] = 0;
	}

	// clear all inserted spatials
	data.clear();
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
			children[octant] = new Octree(this, o, extent * 0.5f, minSize);
		}
		
		// insert spatial into child node
		children[octant]->insert(s);
	}

}

void Octree::select(BoundingVolume* volume, SpatialList result)
{
	result.clear();
	BOOST_FOREACH(SpatialPtr s, data) {
		if(volume->contains(s->getPosition())) {
			result.push_back(s);
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
