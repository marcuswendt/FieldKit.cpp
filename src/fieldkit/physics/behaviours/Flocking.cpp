/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by David Hoe on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/Flocking.h"
using namespace fk::physics;

// -- Align --------------------------------------------------------------------

//  calc average force and move towards it (use velocity if available)..
void FlockAlign::apply(ParticlePtr p)
{
	Vec3f average(0.0f,0.0f,0.0f);
	Particle *q;
	int n;
	
	n = p->neighbours.size();
	list<void *>::iterator it = p->neighbours.begin();
	while( it != p->neighbours.end()) 
	{ 
		q = (Particle *) *it;				
		average += q->force;s
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
	int n = p->neighbours.size();
	list<void *>::iterator it = p->neighbours.begin();
	Particle *q;
	while( it != p->neighbours.end()) 
	{ 
		q = (Particle *) *it;				
		center += *q;	
	}
	if(n >0) center /= (float)n;
	
	Vec3f vec = (*p - center);
	vec.normalize();
	vec *= weight;
	
	p->force += vec;
}


// -- Repel --------------------------------------------------------------------

void FlockRepel::apply(ParticlePtr p)
{
	float distSq, radius, radiusSq, dist;
	Vec3f tmp;
	
	list<void *>::iterator it = p->neighbours.begin();
	Particle *q;
	while( it != p->neighbours.end()) 
	{ 
		q = (Particle *) *it;				
		if(q != p) 
		{
			tmp = *q - *p;
			distSq = tmp.lengthSquared();
			radius = (p->size + q->size) * 0.51f;
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