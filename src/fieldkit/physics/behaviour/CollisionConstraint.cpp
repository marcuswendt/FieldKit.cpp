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
	SpatialListPtr neighbours = p->getNeighbours();

	for(SpatialList::size_type i = 0; i != neighbours->size(); i++)
	{
		Spatial* n = neighbours->operator[](i);
				
		if(p == n) continue;

		// get vector from particle to neighbour
		Vec3f delta = n->getPosition() - p->getPosition();
		float distSq = delta.lengthSquared();
		
		// calc min distance between spatials to they dont overlap
		float radius; 
		
		// particle x particle interaction
		if(n->getType() == Spatial::TYPE_PARTICLE) {
			radius = (p->getSize() + ((Particle*)n)->getSize()) * 0.51f;
			
		// particle x other interaction
		} else {
			radius = p->getSize();
		}
		
		float radiusSq = radius * radius;
		
		// check wether spatials collide
		if(distSq < radiusSq) {
			float dist = sqrtf(distSq);
			delta *= (dist - radius)/ radius * 0.5f;
			p->setPosition(p->getPosition() + delta);
			n->setPosition(n->getPosition() - delta);
		}		
	}
}
