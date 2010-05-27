/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   FlockAttract.cpp
 *	 Created by David Hoe on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/FlockAttract.h"
using namespace fk::physics;

// calculate center of neighbours and move towards it
void FlockAttract::apply(Particle *p)
{
	Vec3f center(0.0f,0.0f,0.0f);
	int n = p->neighbours.size();
	list<void *>::iterator it = p->neighbours.begin();
	Particle *q;
	while( it != p->neighbours.end()) 
	{ 
		q = (Particle *) *it;				
		center += q->position;	
	}
	if(n >0) center /= (float)n;
	
	Vec3f vec = (p->position - center);
	vec.normalize();
	vec *= weight;

	p->force += vec;
}
