/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Behaviour.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include <functional>

namespace fk { namespace physics {

	// forward class declaration
	class Particle;
		
//	class Behaviour : public unary_function<Particle*, void>{
//	public:
////		virtual void operator()(Particle* p) = 0;
////		
////		void apply(Particle* p) {
////			this->operator()(p);
////		}
//		
//		virtual void apply(Particle* p);
//	};
	
	class Behaviour {
	public:
		virtual void apply(Particle* p) = 0;
	};
	
} } // namespace fk::physics