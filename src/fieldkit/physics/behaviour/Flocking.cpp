/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by David Hoe on 27/05/2010.
 */

#include "fieldkit/physics/behaviour/Flocking.h"

using namespace fieldkit::physics;

// -- Flocking base class --------------------------------------------
void FlockBaseBehaviour::prepare(float dt) 
{
	rangeAbs = space->toAbsolute(range);
	rangeAbsSq = rangeAbs * rangeAbs;
}


// -- Align --------------------------------------------------------------------

//  calc average force and move towards it (use velocity if available)..
void FlockAlign::apply(ParticlePtr p)
{
	Vec3f average(0.0f,0.0f,0.0f);
	Particle *q;
	int n;
	Vec3f delta;
	float distSq;
	n = p->getNeighbours().size();
	list<Spatial*>::iterator it = p->getNeighbours().begin();
	while( it != p->getNeighbours().end()) 
	{ 
		q = (Particle *) *it;				
		delta = q->position - p->position ; 
		distSq = delta.lengthSquared();
		if(distSq < rangeAbsSq) 
		{
			average += q->force;
		}
	}
	if(n > 0) average /= (float)n;

	Vec3f vec = average - p->force;
	//vec.normalize();
	vec *= weight;
	p->force += vec;
}


// -- Attract ------------------------------------------------------------------

// calculate center of neighbours and move towards it
void FlockAttract::apply(ParticlePtr p)
{
	Vec3f center(0.0f,0.0f,0.0f);
	int n = p->getNeighbours().size();
	list<Spatial*>::iterator it = p->getNeighbours().begin();
	Particle *q;
	Vec3f delta;
	float distSq;
	while( it != p->getNeighbours().end()) 
	{ 
		q = (Particle *) *it;				
		delta = q->position - p->position ; 
		distSq = delta.lengthSquared();
		if(distSq < rangeAbsSq) 
		{
			center += q->position;	
		}
	}
	if(n >0) center /= (float)n;
	
	Vec3f vec = (p->position - center);
	vec.normalize();
	vec *= weight;
	
	p->force += vec;
}


// -- Repel --------------------------------------------------------------------
void FlockRepel::apply(ParticlePtr p)
{
	float distSq, radius, radiusSq, dist;
	Vec3f tmp;
	
	list<Spatial*>::iterator it = p->getNeighbours().begin();
	ParticlePtr q;
	while(it != p->getNeighbours().end()) 
	{ 
		q = (ParticlePtr) *it;
		if(q->position != p->position) 
		{
			tmp = q->position - p->position;
			distSq = tmp.lengthSquared();
			radius = (p->getSize() + q->getSize()) * 0.51f;
			radiusSq = radius * radius;
			if(distSq < radiusSq) 
			{
				dist = sqrtf(distSq);
				tmp *= (dist - radius)/ radius * 0.5f;
				tmp.normalize();
				tmp *= weight;	
				p->force += tmp;
				//q->force -= tmp
			}
		}	
	}
}