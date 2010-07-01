/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/physics/space/Space.h"

namespace fieldkit { namespace physics {

	//! A simple list-based Space implementation
	class BasicSpace : public Space {
	public:
		BasicSpace();
		~BasicSpace();

		//! empties the entire space contents
		void clear();

		//! adds a single spatial
		void insert(Spatial* s);

		//! selects all spatials within the given bounding volume
		void select(BoundingVolume* volume, list<Spatial*> result);

	protected:
		list<Spatial*> spatials;
	};

} } // namespace fieldkit::physics