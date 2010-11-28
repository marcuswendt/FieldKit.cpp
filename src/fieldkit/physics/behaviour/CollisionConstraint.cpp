/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 25/07/2010.
 */

#include "fieldkit/physics/behaviour/CollisionConstraint.h"

using namespace fieldkit::physics;

void CollisionConstraint::apply(Particle* p) 
{	
	Vec3f delta;
	float dist, distSq, radius, radiusSq;
	Spatial* n;
	SpatialListPtr neighbours = p->getNeighbours();
	int nNeighbours = neighbours->size();

	for(SpatialList::size_type i = 0; i != nNeighbours; i++)
	{
		n = neighbours->operator[](i);
				
		// ignore self
		if(p == n) continue;

		// get vector from particle to neighbour
		delta.set(n->getPosition());
		delta -= p->position;

		//delta = n->getPosition() - p->getPosition();
		distSq = delta.lengthSquared();

		// calc min distance between spatials to they dont overlap
		// particle x particle interaction
		if(n->getType() == Spatial::TYPE_PARTICLE) {
			//radius = (p->getSize() + ((Particle*)n)->getSize()) * 0.51f;
			radius = (p->size + ((Particle*)n)->size) * 0.51f;
			
		// particle x other interaction
		} else {
			radius = p->getSize();
		}
		
		radiusSq = radius * radius;
		
		// check whether spatials collide
		if(distSq < radiusSq) {
			dist = sqrtf(distSq);
			delta *= (dist - radius)/ radius * 0.5f * bouncyness;
			p->setPosition(p->getPosition() + delta);
			n->setPosition(n->getPosition() - delta);
		}		
	}
}
