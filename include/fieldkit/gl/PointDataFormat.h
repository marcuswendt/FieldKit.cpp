/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/gl/GLKit_Prefix.h"
#include <string>
#include <vector>

namespace fieldkit { namespace gl {

	class PointDataFormat {
	public:
		PointDataFormat() {};
		~PointDataFormat() {};
		
		void clear();
		void addAttribute(std::string name, int size, int type=GL_FLOAT);
						  
//	protected:
		//! defines a single attribute
		struct Attribute {
            std::string name;
			
			//! variable type of this attribute (GL_INT, GL_FLOAT)
			int type;
			
			//! number of elements 
			int size;
			
			//! size in bytes (depends on type)
			int bytes;
		};

        std::vector<PointDataFormat::Attribute> attributes;
	};
} } // namespace fieldkit::gl