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

void CollisionConstraint::apply(Particle* p) {
	
	//SpatialList::iterator it = p->getNeighbours()->begin();
	//while( it != p->getNeighbours()->end())
	
	SpatialList::iterator it;
	for(it=p->getNeighbours()->begin(); it < p->getNeighbours()->end(); it++ )
	{
		Spatial* n = *it;
//		logger() << "CollisionConstraint::apply p:" << p->getPosition() 
//				 << " n:" << n->getPosition() << endl;
		
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
			float dist = sqrt(distSq);
			delta *= (dist - radius)/ radius * 0.5f;
			p->setPosition(p->getPosition() + delta);
			n->setPosition(n->getPosition() - delta);
		}		
	}
}