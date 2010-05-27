/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   FlockAlign.cpp
 *	 Created by David Hoe on 27/05/2010.
 */

#include "fieldkit/physics/behaviours/FlockAlign.h"
using namespace fk::physics;

//  calc average force and move towards it (use velocity if available)..
void FlockAlign::apply(Particle *p)
{
	Vec3f average(0.0f,0.0f,0.0f);
	Particle *q;
	int n;
	
	n = p->neighbours.size();
	list<void *>::iterator it = p->neighbours.begin();
	while( it != p->neighbours.end()) 
	{ 
		q = (Particle *) *it;				
		average += q->force;
	}
	if(n > 0) average /= (float)n;

	Vec3f vec = average - p->force;
	//vec.normalize();
	vec *= weight;
	p->force += vec;
}
