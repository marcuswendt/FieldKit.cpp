/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/07/2010.
 */

#include "fieldkit/physics/space/SpatialHash.h"

#include "fieldkit/math/MathKit.h"
using namespace fieldkit::math;

using namespace fieldkit::physics;

SpatialHash::SpatialHash()
{
	init(Vec3f::zero(), Vec3f(100.0f, 100.0f, 100.0f), 10.0f);	
}

SpatialHash::SpatialHash(Vec3f offset, Vec3f dimension, float cellSize)
{
	init(offset, dimension, cellSize);	
}

SpatialHash::~SpatialHash()
{
	if(ownsSpatials) {
		int n = cells.size();
		for(int i=0; i<n; i++) {
			BOOST_FOREACH(Spatial* spatial, cells[i]) {
				delete spatial;
				spatial = NULL;
			}
			cells[i].clear();
		}
		cells.clear();
	}
}

void SpatialHash::init(Vec3f offset, Vec3f dimension, float cellSize)
{
	// init bounds
	this->position = offset + dimension * 0.5f;
	this->extent = dimension * 0.5f;
	updateBounds();
	
	// create cells
	this->cellSize = cellSize;
	cellsX = hash(dimension.x);
	cellsY = hash(dimension.y);
	
	// reserve cells
	cells.resize(cellsX * cellsY);
}

void SpatialHash::clear() 
{
	int n = cells.size();
	for(int i = 0; i < n; i++) {
		cells[i].clear();
	}
}

void SpatialHash::insert(Spatial* spatial) 
{
	// find position in cell space
	Vec3f p = spatial->getPosition() - (this->position - this->extent);
	int hashX = hash(p.x);
	int hashY = hash(p.y);
	
	// make sure the spatial lies within the cell space
	unsigned int index = hashY * cellsX + hashX;
	if(index < 0 || index >= cells.size()) return;
	cells[index].push_back(spatial);
}

void SpatialHash::select(BoundingVolume* volume, SpatialListPtr result)
{
	// find search center position in cell space
	Vec3f p = volume->getPosition() - (this->position - this->extent);
	int hashX = hash(p.x);
	int hashY = hash(p.y);
	
	// figure out search radius
	int searchX = 0;
	int searchY = 0;
	switch(volume->getType()) {
		case BOUNDING_BOX: {
			AABB* box = (AABB*)volume;
			searchX = hash(box->getWidth() * 0.5f);
			searchY = hash(box->getHeight() * 0.5f);
			break;
		}
			
		case BOUNDING_SPHERE: {
			SphereBound* sphere = (SphereBound*)volume;
			searchX = searchY = hash(sphere->getRadius());
			break;
		}
	};
	
	// make sure we have a clean list
	result->clear();
	
	int sx = std::max(hashX-searchX, 0);
	int sy = std::max(hashY-searchY, 0);
	int ex = std::min(hashX+searchX, cellsX);
	int ey = std::min(hashY+searchY, cellsY);
	
	// put all spatials from the selected cells into result
	int index;
	for(int i=sy; i<ey; i++) {
		for(int j=sx; j<ex; j++) {
			index = i * cellsX + j;
			for(SpatialList::size_type i = 0; i != cells[index].size(); i++) {
				result->push_back(cells[index][i]);
			}
		}
	}
}
