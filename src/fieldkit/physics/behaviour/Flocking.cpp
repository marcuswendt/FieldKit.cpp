/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by David Hoe on 27/05/2010.
 *	 Updates by Marcus Wendt on 23/09/2010.
 */

#include "fieldkit/physics/behaviour/Flocking.h"

using namespace fieldkit::physics;

void FlockingBehaviour::prepare(float dt) 
{
	rangeAbs = space->toAbsolute(range);
	rangeAbsSq = rangeAbs * rangeAbs;
}


// Attract - calculate center of neighbours and move towards it
void FlockAttract::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	Vec3f average(0.0f,0.0f,0.0f);
	Vec3f delta;
	float distSq;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;

		Particle* n = (Particle*)s;
		delta = n->position - p->position; 
		distSq = delta.lengthSquared();
		if(distSq > EPSILON && distSq < rangeAbsSq)
			average += delta;
	}

	// calculate average and attract towards it
	average /= (float)nNeighbours;

	// check length, avoid division by zero!
	float lenSq = average.lengthSquared();
	if(lenSq <= EPSILON) return;

	average /= sqrt(lenSq);
	average *= weight;

	p->force += average;
}


//! Align - Calculate average force and move towards it (use velocity if available).
void FlockAlign::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	Vec3f average(0.0f,0.0f,0.0f);
	Vec3f delta;
	float distSq;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;
		
		Particle* n = (Particle*)s;
		delta = n->position - p->position; 
		distSq = delta.lengthSquared();
		if(distSq > EPSILON && distSq < rangeAbsSq)
			average += n->getVelocity();
	}

	// calculate average and add to force
	average /= (float)nNeighbours;

	// check length, avoid division by zero!
	float lenSq = average.lengthSquared();
	if(lenSq <= EPSILON) return;

	average /= sqrt(lenSq);
	average *= weight;
	
	p->force += average;
}


//! Repel - move away from all neighbours colliding with particle
void FlockRepel::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	Vec3f average(0.0f,0.0f,0.0f);
	Vec3f delta;
	float distSq;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;

		Particle* n = (Particle*)s;
		delta = n->position - p->position; 
		distSq = delta.lengthSquared();
		if(distSq > EPSILON && distSq < rangeAbsSq)
			average += delta;
	}

	// calculate average and repel from it
	average /= (float)nNeighbours;

	// check length, avoid division by zero!
	float lenSq = average.lengthSquared();
	if(lenSq <= EPSILON) return;

	average /= sqrt(lenSq);
	average *= weight * -1.0f; // negative attract

	p->force += average;
}