/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Behavioural.h"

using namespace fieldkit::physics;
	
void Behavioural::addBehaviour(BehaviourPtr b) {
	behaviours.push_back(b);
}
	
void Behavioural::removeBehaviour(BehaviourPtr b) {
	behaviours.remove(b);
}
	
void Behavioural::addConstraint(ConstraintPtr c) {
	constraints.push_back(c);
}
	
void Behavioural::removeConstraint(ConstraintPtr c) {
	constraints.remove(c);
}