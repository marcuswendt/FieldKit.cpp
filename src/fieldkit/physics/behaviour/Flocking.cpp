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


//! Align - Calculate average force and move towards it (use velocity if available).
void FlockAlign::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	Vec3f average(0.0f,0.0f,0.0f);
	Vec3f delta;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;

		// do not apply to itself
		if(s != p) continue;

		Particle* n = (Particle*)s;
		delta = n->position - p->position; 
		if(delta.lengthSquared() < rangeAbsSq)
			average += n->getVelocity();
	}

	// calculate average and add to force
	average /= (float)nNeighbours;

	p->force += average.normalized() *= weight;
}


// Attract - calculate center of neighbours and move towards it
void FlockAttract::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	Vec3f center(0.0f,0.0f,0.0f);
	Vec3f delta;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;
		
		// do not apply to itself
		if(s != p) continue;

		Particle* n = (Particle*)s;
		delta = n->position - p->position; 
		if(delta.lengthSquared() < rangeAbsSq)
			center += n->position;
	}

	// calculate average and attract towards it
	center /= (float)nNeighbours;

	p->force += (p->position - center).normalized() *= weight;
}


//! Repel - move away from all neighbours colliding with particle
void FlockRepel::apply(Particle* p)
{
	// check if particle has neighbours at all
	int nNeighbours = p->getNeighbours()->size();
	if(nNeighbours == 0) return;

	float distSq, radius, radiusSq, dist;
	Vec3f tmp;

	// check radius, only apply to particle spatials
	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		Spatial* s = p->getNeighbours()->operator[](i);
		if(s->getType() != Spatial::TYPE_PARTICLE) continue;

		// do not apply to itself
		if(s != p) continue;

		Particle* n = (Particle*)s;
		
		tmp = n->position - p->position; 
		distSq = tmp.lengthSquared();
		
		radius = (p->getSize() + n->getSize()) * 0.51f;
		radiusSq = radius * radius;

		if(distSq < radiusSq) {
			dist = sqrtf(distSq);

			tmp *= (dist - radius)/ radius * 0.5f;
			tmp.normalize();
			tmp *= weight;	

			p->force += tmp;
		}
	}
}