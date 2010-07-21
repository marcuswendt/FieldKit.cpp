/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/07/2010.
 */

#include "fieldkit/physics/space/SpatialHash.h"

using namespace fieldkit::physics;

// -- SpatialHash::Cell --------------------------------------------------------
SpatialHash::Cell::Cell() 
{
	isEmpty=true;
}

SpatialHash::Cell::~Cell() 
{
}

void SpatialHash::Cell::clear()
{
	spatials.clear();
	isEmpty=true;
}

void SpatialHash::Cell::insert(SpatialPtr spatial)
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
	destroy();
}

void SpatialHash::init(Vec3f offset, Vec3f dimension, float cellSize)
{
	// init bounds
	this->position = offset + dimension;
	this->extent = dimension;
	updateBounds();

	// create cells
	this->cellSize = cellSize;
	cellsX = (int)(dimension.x / cellSize);
	cellsY = (int)(dimension.y / cellSize);

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
	for(int i = 0; i < cellsX; i++) {
		for(int j = 0; j < cellsY; j++) {
			delete cells[i][j];
		}
		delete[] cells[i];
	}
	delete[] cells;
	cells = NULL;
}	

void SpatialHash::clear() 
{
	for(int i=0; i < cellsX; i++) {
		for(int j=0; j < cellsY; j++) {
			cells[i][j]->clear();
		}
	}	
}

void SpatialHash::insert(SpatialPtr spatial) 
{
	Vec3f p = spatial->getPosition();
	int hashX = (int)(p.x / cellSize);
	int hashY = (int)(p.y / cellSize);
	if(hashX > 0 && hashX < cellsX && 
	   hashY > 0 && hashY < cellsY) {\
		cells[hashX][hashY]->insert(spatial);
	} else {
		
	}
}

void SpatialHash::select(BoundingVolumePtr volume, SpatialListPtr result)
{
	
}

