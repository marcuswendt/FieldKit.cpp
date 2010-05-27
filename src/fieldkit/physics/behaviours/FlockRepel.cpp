/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   FlockRepel.cpp
 *	 Created by David Hoe on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/FlockRepel.h"
using namespace fk::physics;

void FlockRepel::apply(Particle *p)
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
			tmp = q->position - p->position;
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