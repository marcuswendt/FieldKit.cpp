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

// -- SpatialHash::Cell --------------------------------------------------------
SpatialHash::Cell::Cell() 
{
	isEmpty=true;
}

SpatialHash::Cell::~Cell() {}

void SpatialHash::Cell::clear()
{
	spatials.clear();
	isEmpty=true;
}

void SpatialHash::Cell::insert(Spatial* spatial)
{
	spatials.push_back(spatial);
	isEmpty=false;
}


// -- SpatialHash --------------------------------------------------------------
SpatialHash::SpatialHash()
{
	cells = NULL;
	init(Vec3f::zero(), Vec3f(100.0f, 100.0f, 100.0f), 10.0f);	
}

SpatialHash::SpatialHash(Vec3f offset, Vec3f dimension, float cellSize)
{
	cells = NULL;
	init(offset, dimension, cellSize);	
}

SpatialHash::~SpatialHash()
{
	//fk::logger() << "SpatialHash::~SpatialHash()" << std::endl;
	destroy();
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

	if(cells != NULL) 
		destroy();
	
	cells = new CellList[cellsX];
	for(int i=0; i < cellsX; i++) {
		cells[i] = new CellPtr[cellsY];
		for(int j=0; j < cellsY; j++) {
			cells[i][j] = CellPtr(new Cell());
		}
	}
}

void SpatialHash::destroy()
{
	for(int i=0; i<cellsX; i++) {
		for(int j=0; j<cellsY; j++) {

			// delete a single cell
			SpatialHash::Cell* cell = cells[i][j];

			// if owner also delete its spatials
			if(ownsSpatials) {
				BOOST_FOREACH(Spatial* spatial, cell->spatials) {
					if(spatial != NULL) {
						delete spatial;
						spatial = NULL;
					}
				}
			}

			delete cell;
			cell = NULL;
		}
		delete[] cells[i];
	}
	delete[] cells;
	cells = NULL;
}	

void SpatialHash::clear() 
{
	for(int i=0; i<cellsX; i++) {
		for(int j=0; j<cellsY; j++) {
			cells[i][j]->clear();
		}
	}	
}

void SpatialHash::insert(Spatial* spatial) 
{
	// find position in cell space
	Vec3f p = spatial->getPosition();
	int hashX = hash(p.x);
	int hashY = hash(p.y);

	// make sure the spatial lies within the cell space
	if(hashX >= 0 && hashX < cellsX && 
	   hashY >= 0 && hashY < cellsY) {
		cells[hashX][hashY]->insert(spatial);
	}
}

void SpatialHash::select(BoundingVolume* volume, SpatialListPtr result)
{
	// find search center position in cell space
	Vec3f p = volume->getPosition();
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
//	for(int i=hashY-searchY; i<hashY+searchY; i++) {
//		for(int j=hashX-searchX; j<hashX+searchX; j++) {
	for(int i=sy; i<ey; i++) {
		for(int j=sx; j<ex; j++) {
			// check wether we're still inside the grid
//			if(j >= 0 && j < cellsX && 
//			   i >= 0 && i < cellsY) {
				CellPtr cell = cells[j][i];
				BOOST_FOREACH(Spatial* spatial, cell->spatials) {
					result->push_back(spatial);
				}
//			}			
		}
	}
}